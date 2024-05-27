/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rverhoev <rverhoev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:18:38 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/27 08:53:48 by rverhoev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "miniRT.h"
#include "../minilibx-linux/mlx.h"

void init_ray_send_tools(t_ray_sending_tools *r_t, t_data *data)
{
	r_t->half_screen_width = WINDOW_WIDTH * 0.5;
	r_t->half_screen_height = WINDOW_HEIGHT * 0.5;
	r_t->perpendicular_distance_horiz_triangle = r_t->half_screen_width / (float)tan(r_t->start_angle_horiz);
	r_t->perpendicular_distance_vert_triangle = r_t->half_screen_height / (float)tan(r_t->start_angle_vert);
}

void set_rgb_factor(float *rgb_factor)
{
	*rgb_factor += 1;
	*rgb_factor *= 0.5;
}

int visualize_sphere_normals(t_data *data, float res_xyz[3])
{
	float rgb_factor[3];

	normalize_vector(res_xyz);
	copy_matrix(rgb_factor, res_xyz);
	set_rgb_factor(&rgb_factor[0]);
	set_rgb_factor(&rgb_factor[1]);
	set_rgb_factor(&rgb_factor[2]);
	// printf("factor\n");
	// print_matrix_1_3(rgb_factor);
	// printf("________________________\n");
	// return (create_color(interpolate(BLUE, ORANGE, rgb_factor[0]) << 16, interpolate(RED, WHITE, rgb_factor[1]) << 8, interpolate(GREEN, BLACK, rgb_factor[2])));
	int const color1 = interpolate(BLUE, WHITE, rgb_factor[0]);
	int const color2 = interpolate(RED, color1, rgb_factor[1]);
	int const color3 = interpolate(GREEN, color2, rgb_factor[2]);

	return (color3);
	// return (interpolate(BLUE, ORANGE, rgb_factor[0]));
	// return (create_color((int)((float)255 * rgb_factor[0]) & 0xFF, (int)((float)255 * rgb_factor[1]) & 0xFF, (int)((float)255 * rgb_factor[2]) & 0xFF));
	//255 * rgb_factor[0]
}

int hit_object(t_data *data)
{
	float res_xyz[3];

	// printf("ray direction\n"); 
	// print_matrix_1_3(data->ray.scaled_vector);
	// printf("obj center\n"); 
	// print_matrix_1_3(data->sphere.object_center_xyz);
	res_xyz[0] = (data->ray.scaled_vector[0] - data->camara.pos_xyz[0])
		- data->sphere.object_center_xyz[0];
 	res_xyz[1] = (data->ray.scaled_vector[1] - data->camara.pos_xyz[1])
		- data->sphere.object_center_xyz[1];
	res_xyz[2] = (data->ray.scaled_vector[2] - data->camara.pos_xyz[2])
		- data->sphere.object_center_xyz[2];
	// sleep(1);
	// printf("result xyz %f %f %f\n", res_xyz[0], res_xyz[1], res_xyz[2]);

	double squared = pow((double)res_xyz[0], 2) + pow((double)res_xyz[1], 2) + pow((double)res_xyz[2], 2);
	// printf("sqrt(squared) %f\n", sqrt(squared));
	// printf("radius %f\n",  data->sphere.radius);
	// sleep(1);
	if (sqrt(squared) <= (double)data->sphere.radius)
	{
		// visualize_sphere_normals(data, res_xyz);
		return (visualize_sphere_normals(data, res_xyz));
		// return (data->sphere.color);
	}
	else
		return ((int)NADA);
}

int	hit_ray(t_data *data, float angle_horiz, float angle_vert)
{
	float	rota_horiz[3][3];
	float	rota_vert[3][3];
	float	comp[3][3];

	init_t_around_z(rota_horiz, angle_horiz);
	init_t_around_y(rota_vert, angle_vert);
	compilation_matrix(comp, rota_horiz, rota_vert);
	// if (PRINT_DEBUG) printf("angles horizontal, vertical: %f\t%f\n", angle_horiz, angle_vert);

	// if (PRINT_DEBUG) printf("_________________\n");
	// if (PRINT_DEBUG) printf("rotation\n");

	// if (PRINT_DEBUG) print_matrix_3_3(comp);

	// if (PRINT_DEBUG) printf("original\n");
	// if (PRINT_DEBUG) print_matrix_1_3(data->camara.view_orientation_matrix);
	init_result(data->ray.direction_abc);
	matrix_multiplication(comp, &data->ray, data->camara.view_orientation_matrix);
	// if (PRINT_DEBUG) printf("result:\n");
	// if (PRINT_DEBUG) print_matrix_1_3(data->ray.direction_abc);
	// usleep(100);
	// if (PRINT_DEBUG) printf("_________________\n\n");
	data->ray.vector_scalar_step = 1;
	int step = 2;
	// if (PRINT_DEBUG) printf("scaled:\n");
	while (step < 200)
	{
		int hit_result;
		copy_matrix(data->ray.scaled_vector, data->ray.direction_abc);
		vector_scaling(data->ray.scaled_vector, (float)step);
		// if (angle_horiz > -0.05 && angle_horiz < 0.05)
		hit_result = hit_object(data);
		if (hit_result != NADA)
		{
			// normalize_vector(data->ray.direction_abc);
			// printf("hit at xyz: :\n");
			// print_matrix_1_3(data->ray.scaled_vector);
			return (hit_result);
		}
		step += 2;
		// if (PRINT_DEBUG) print_matrix_1_3(data->ray.direction_abc);
	}
	// if (angle_horiz > 0 && angle_horiz < 0.09)
	// 	printf("%f\n", data->ray.direction_abc[2]);
	return (NADA);
}

void	send_rays(t_data *data)
{
	t_ray_sending_tools	r_t;
	int					color;
	clock_t				t;

   	t = clock();
	init_ray_send_tools(&r_t, data);
	r_t.pixel_y = 0;
	while (r_t.pixel_y <= WINDOW_HEIGHT)
	{
		r_t.pixel_x = 0;
		r_t.angle_vert = atan2(-r_t.half_screen_height + r_t.pixel_y, r_t.perpendicular_distance_vert_triangle);
		while (r_t.pixel_x <= WINDOW_WIDTH)
		{
			r_t.angle_horiz = atan2(-r_t.half_screen_width + r_t.pixel_x, r_t.perpendicular_distance_horiz_triangle);
			color = hit_ray(data, r_t.angle_horiz, r_t.angle_vert);
			if (color == NADA)
			{
				float unit_point;
				unit_point = (r_t.angle_vert - r_t.start_angle_vert) / data->camara.field_of_view_rad;
				color = interpolate(WHITE, BLUE, unit_point);
			}
			put_pixel_img(data->image, r_t.pixel_x, r_t.pixel_y, color);
			r_t.pixel_x++;
		}
		r_t.pixel_y++;
	}
	t = clock() - t;
   	double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
   	printf("The program took %f seconds to execute\n", time_taken);
	printf("done\n");
}
/*
printf("perp hori %f\n", r_t.perpendicular_distance_horiz_triangle);
printf("perp verti %f\n", r_t.perpendicular_distance_vert_triangle);

printf("angle hori verti: %f\t%f\n", r_t.angle_horiz, r_t.angle_vert);
sleep(1);

printf("angle hori verti: %f\t%f\n", angle_horiz, angle_vert);
printf("xy: %d\t%d\n", pixel_x, pixel_y);
*/