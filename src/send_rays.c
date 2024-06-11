/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:18:38 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/06/11 10:09:39 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "miniRT.h"
#include "../minilibx-linux/mlx.h"

void init_ray_send_tools(t_ray_sending_tools *r_t, t_data *data)
{
	r_t->half_screen_width = WINDOW_WIDTH * 0.5;
	r_t->half_screen_height = WINDOW_HEIGHT * 0.5;
	r_t->start_angle_horiz = data->camara.field_of_view_rad * 0.5;
	r_t->start_angle_vert = data->camara.field_of_view_rad * 0.5;
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
}

/**
 * vector from point A to point B
*/
t_vec	points_derived_vector(float point_A[3], float point_B[3])
{
	t_vec	result_vec;

	result_vec.m[0] = point_B[0] - point_A[0]; 
 	result_vec.m[1] = point_B[1] - point_A[1];  
	result_vec.m[2] = point_B[2] - point_A[2];  
	return (result_vec);
}

void moved_vector_position(float result[3], t_vec target_vector, int offset[3])
{
	result[0] = target_vector.m[0] - offset[0]; 
 	result[1] = target_vector.m[1] - offset[1];  
	result[2] = target_vector.m[2] - offset[2];
}

int	sphere_intersection_equation(t_data *data, t_vec obj_to_ray_vec)
{
	const float squared = powf(obj_to_ray_vec.m[0], 2) + powf(obj_to_ray_vec.m[1], 2) + powf(obj_to_ray_vec.m[2], 2);

	if (sqrtf(squared) <= data->sphere.radius)
		return (TRUE);
	else
		return (FALSE);
}

float	dot_product_3d(float vec_A[3], float vec_B[3])
{
	return ((vec_A[0] * vec_B[0]) + (vec_A[1] * vec_B[1]) + (vec_A[2] * vec_B[2]));
}

float	cross_product_3d(float vec_A[3], float vec_B[3])
{
	return ((vec_A[2] * vec_B[3]) - (vec_B[2] * vec_A[3]) +
			(vec_A[3] * vec_B[1]) - (vec_B[3] * vec_A[1]) +
			(vec_A[1] * vec_B[2]) - (vec_B[1] * vec_A[2]));
}


/**
 * @note not declaring and initializing new vectors faster??
*/
int check_if_hit(t_data *data, t_ray *ray, t_vec *obj_to_ray_vec)
{
	moved_vector_position(ray->world_pos_of_scaled_vec, ray->scaled_vec, data->camara.pos_xyz);
	// print_matrix_1_3(ray->world_pos_of_scaled_vec);
	
	*obj_to_ray_vec = points_derived_vector(data->sphere.object_center_xyz, ray->world_pos_of_scaled_vec);
	// printf("ray scaled\n"); 
	// print_matrix_1_3(ray->scaled_vec.m);
	// printf("obj to ray vec\n");
	// print_matrix_1_3(obj_to_ray_vec.m);
	// sleep(1);
	if (sphere_intersection_equation(data, *obj_to_ray_vec) == TRUE)
	{
		// visualize_sphere_normals(data, res_xyz);
		// return (visualize_sphere_normals(data, (*obj_to_ray_vec).m));
		// int shadow_check = shadow_ray_to_light();
		// return ();
		return (data->sphere.color);
	}
	else
		return ((int)NADA);
	// usleep(5000);
	// printf("result xyz %f %f %f\n", res_xyz[0], res_xyz[1], res_xyz[2]);
	// printf("sqrt(squared) %f\n", sqrt(squared));
	// printf("radius %f\n",  data->sphere.radius);
	// sleep(1);
}

int	shadow_ray_to_light(t_data *data)
{

	float	position[3];
	float	light_ray[3];

// 	int steps = 0;

// 	position[0] = surface_point[0] + light_ray[0];
// 	position[1] = surface_point[1] + light_ray[1];
// 	position[2] = surface_point[2] + light_ray[2];
// 	while (steps < 200) // ray in object? = shadow
// 	{
// 		position[0] += light_ray[0];
// 		position[1] += light_ray[1];
// 		position[2] += light_ray[2];
// 		// if (PRINT_DEBUG) print_matrix_1_3(position);
// 		if (check_if_hit(data, position))
// 			return (FALSE);
// 		steps++;
// 	}
	return (TRUE);
}

void	increment_vec(t_vec *scaled_vector, t_vec *normalize_vector)
{
	(*scaled_vector).m[0] += (*normalize_vector).m[0];
	(*scaled_vector).m[1] += (*normalize_vector).m[1];
	(*scaled_vector).m[2] += (*normalize_vector).m[2];
}

// float	*int_1x3_to_float_1x3(int i_arr[3])
// {
// 	float	f_arr[3];
// 	int		i;

// 	i = 0;	
// 	while (i < 3)
// 	{
// 		f_arr[i] = i_arr[i];
// 		i++;
// 	}
// 	return (f_arr);
// }

int	hit_ray(t_data *data, float angle_horiz, float angle_vert)
{
	float	rota_horiz[3][3];
	float	rota_vert[3][3];
	float	comp[3][3];

	init_t_around_z(rota_horiz, angle_horiz);
	init_t_around_y(rota_vert, angle_vert);
	compilation_matrix(comp, rota_horiz, rota_vert);

	// if (PRINT_DEBUG) printf("angles horizontal, vertical: %f\t%f\n", ft_rad_to_degr(angle_horiz), ft_rad_to_degr(angle_vert));
	// if (PRINT_DEBUG) printf("_________________\n");
	// if (PRINT_DEBUG) printf("rotation\n");
	// if (PRINT_DEBUG) print_matrix_3_3(comp);
	// if (PRINT_DEBUG) printf("original\n");
	// if (PRINT_DEBUG) print_matrix_1_3(data->camara.view_orientation_matrix);

	init_result(data->ray.normalized_vec.m);

	matrix_multiply_1x3_3x3(data->camara.view_orientation_matrix, comp, data->ray.normalized_vec.m);

	// if (PRINT_DEBUG) printf("result:\n");
	// if (PRINT_DEBUG) print_matrix_1_3(data->ray.normalized_vec.m);
	// if (PRINT_DEBUG) printf("_________________\n\n");
	// sleep(2);
	data->ray.step = 1;
	init_result(data->ray.scaled_vec.m);

	while (data->ray.step < 200)
	{
		int resulting_color;

		t_sphere_object hit_object;
		t_vec			obj_to_ray_vec;
		// sleep(1);

		increment_vec(&data->ray.scaled_vec, &data->ray.normalized_vec);

		resulting_color = check_if_hit(data, &data->ray, &obj_to_ray_vec);
		// if (dot_product_3d())
		if (resulting_color != NADA)
		{
			t_vec surface_normal;
			
			copy_matrix(surface_normal.m, obj_to_ray_vec.m);
			normalize_vector(surface_normal.m);
			float	*surface_point = data->ray.scaled_vec.m;// must be actual surface point, not in the object! like now
			// print_matrix_1_3(data->ray.scaled_vec.m);
			// print_matrix_1_3(data->light.position);

			t_vec surface_to_light_ray = points_derived_vector(data->ray.scaled_vec.m, data->light.position);
			normalize_vector(surface_to_light_ray.m);
			// printf("surface normal:\n");
			// print_matrix_1_3(surface_normal.m);
			// printf("surface to light:\n");
			// print_matrix_1_3(surface_to_light_ray.m);
			// printf(" ______:\n");
			// sleep(1);
			float one[3] = {200, 200, 200};
			float two[3] = {-100, -100, -100};
			// printf("dot product %f\n_______\n", dot_product_3d(one, two));

			// printf("dot product %f\n", dot_product_3d(surface_normal.m, surface_to_light_ray.m));
			if (dot_product_3d(surface_normal.m, surface_to_light_ray.m) < 0)
				return (0);
			else
				return (resulting_color);
		}
		data->ray.step += 1;
		// if (PRINT_DEBUG) print_matrix_1_3(data->ray.scaled_vec.m);
	}
	// if (angle_horiz > 0 && angle_horiz < 0.09)
	// 	printf("%f\n", data->ray.direction_abc[2]);
	return (NADA);
}

// int	world_horizon_opposed_to_ray(t_data *data)
// {
// }

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
		r_t.angle_vert = atan2(r_t.half_screen_height - r_t.pixel_y, r_t.perpendicular_distance_vert_triangle);
		while (r_t.pixel_x <= WINDOW_WIDTH)
		{
			r_t.angle_horiz = atan2(-r_t.half_screen_width + r_t.pixel_x, r_t.perpendicular_distance_horiz_triangle);
			// if (PRINT_DEBUG) printf("angles horizontal, vertical: 1  %f\t%f\n", ft_rad_to_degr(r_t.angle_horiz), ft_rad_to_degr(r_t.angle_vert));
			// printf("perp hori %f\n", r_t.perpendicular_distance_horiz_triangle);
			// printf("perp verti %f\n", r_t.perpendicular_distance_vert_triangle);
			// printf("screen half %d\n, ", r_t.half_screen_width);
			// sleep(1);
			color = hit_ray(data, r_t.angle_horiz, r_t.angle_vert);
			if (color == NADA)
			{
				float unit_point;

				// unit_point = world_horizon_opposed_to_ray(data);
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