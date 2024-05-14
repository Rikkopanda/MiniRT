/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:06:23 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/13 20:42:22 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minilibx-linux/mlx.h"
#include "stddef.h"
#include "stdio.h"
#include <stdlib.h>
#include "../include/miniRT.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

/**
 * mlxinit prepares essential data bts
*/
t_win	new_window(int w, int h, char *str)
{
	void	*mlx_ptr;

	mlx_ptr = mlx_init();
	return ((t_win){mlx_ptr, mlx_new_window(mlx_ptr, w, h, str), w, h});
}

t_img	new_img(int w, int h, t_win window)
{
	t_img	image;

	image.win = window;
	image.img_ptr = mlx_new_image(window.mlx_ptr, w, h);
	image.addr = mlx_get_data_addr(image.img_ptr, &(image.bpp),
			&(image.line_len), &(image.endian));
	image.w = w;
	image.h = h;
	return (image);
}

void	destroy_image(t_img img)
{
	if (img.img_ptr && img.win.mlx_ptr)
		mlx_destroy_image(img.win.mlx_ptr, img.img_ptr);
}

int handle_input(int keysym, t_data *data)
{
	if (keysym == ON_ESC)
		return (mlx_loop_end(data->mlx.mlx_ptr), -1);
    return (0);
}

void	put_pixel_img(t_img img, int x, int y, int color)
{
	char	*dst;

	if (color == (int)0xFF000000)
		return ;
	if (x >= 0 && y >= 0 && x < img.w && y < img.h)
	{
		dst = img.addr + (y * img.line_len + x * (img.bpp / 8));
		*(unsigned int *) dst = color;
	}
}

int	check_bounds(int x, int y)
{
	if ((x <= WINDOW_WIDTH && x >= 0)
	&& (y <= WINDOW_HEIGHT && y >= 0))
	{
		return (IN_BOUNDS);
	}
	return (OUT_OF_BOUNDS);
}

void draw_square(t_data	*data, int start_xy[2], int size_len_width[2])
{
	int	x;
	int y;

	x = start_xy[0];
	y = start_xy[1];

	while (x <= start_xy[0] + size_len_width[0])
	{
		y = 0;
		while (y <= start_xy[1] + size_len_width[1])
		{
			if (check_bounds(x, y) == IN_BOUNDS)
				put_pixel_img(data->image, x, y, (int)GREEN);
			y++;
		}
		x++;
	}
}

void	init_sphere(t_data *data)
{
	data->sphere.radius = 20;
	data->sphere.color = BLUE;
	data->sphere.object_center_xyz[0] = 200;
	data->sphere.object_center_xyz[1] = 0;
	data->sphere.object_center_xyz[2] = 0;
}

void	init_camara(t_data *data)
{
	data->camara.field_of_view_degrees = 70;
	data->camara.view_orientation_matrix[0] = 1;
	data->camara.view_orientation_matrix[1] = 0;
	data->camara.view_orientation_matrix[2] = 0;
	data->camara.pos_xyz[0] = 0;
	data->camara.pos_xyz[1] = 0;
	data->camara.pos_xyz[2] = 0;
}

void	init_cylinder(t_data *data)
{
	data->cylinder.diameter = 50;
	data->cylinder.color = RED;
	data->cylinder.height = 50;
	data->cylinder.object_center_xyz[0] = 200;
	data->cylinder.object_center_xyz[1] = 200;
	data->cylinder.object_center_xyz[2] = 100;
	// data->cylinder.axis_vector_abc[0] = -1;
}

void	copy_matrix(float dst[3], float src[3])
{
	int i;

	i = 0;
	while (i < 3)
	{
		dst[i] = src[i];
		i++;
	}
}
// copy_matrix(data->ray.direction_abc, data->camara.view_orientation_matrix);


void	print_matrix_1_3(float m[3])
{
	int i;

	i = 0;
	while (i < 3)
	{
		printf("%f\t", m[i]);
		i++;
	}
	printf("\n");
	// printf("_________________\n");
}

void	print_matrix_3_3(float m[3][3])
{
	int i;
	int j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			printf("%f\t", m[i][j]);
			j++;
		}
		i++;
		printf("\n");
	}
}

int hit_object(t_data *data)
{
	float res_xyz[3];

	// printf("ray direction\n"); 
	// print_matrix_1_3(data->ray.scaled_vector);
	// printf("obj center\n"); 
	// print_matrix_1_3(data->sphere.object_center_xyz);

	res_xyz[0] = data->ray.scaled_vector[0]
		- data->sphere.object_center_xyz[0];
 	res_xyz[1] = data->ray.scaled_vector[1]
		- data->sphere.object_center_xyz[1];
	res_xyz[2] = data->ray.scaled_vector[2]
		- data->sphere.object_center_xyz[2];
	// sleep(1);
	// printf("result xyz %f %f %f\n", res_xyz[0], res_xyz[1], res_xyz[2]);

	double squared = pow((double)res_xyz[0], 2) + pow((double)res_xyz[1], 2) + pow((double)res_xyz[2], 2);
	// printf("sqrt(squared) %f\n", sqrt(squared));
	// printf("radius %f\n",  data->sphere.radius);
	// sleep(1);
	if (sqrt(squared) <= (double)data->sphere.radius)
		return (data->sphere.color);
	else
		return ((int)NADA);
}

int	hit_ray(t_data *data, float angle_horiz, float angle_vert)
{
	float	rota_horiz[3][3];
	float	rota_vert[3][3];
	float	comp[3][3];

	init_t_around_z(rota_horiz, ft_degr_to_rad(angle_horiz));
	init_t_around_y(rota_vert, ft_degr_to_rad(angle_vert));
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
		copy_matrix(data->ray.scaled_vector, data->ray.direction_abc);
		vector_scaling(&data->ray, (float)step);
		int hit_result = hit_object(data);
		if (hit_result != NADA)
		{
			// printf("hit at xyz: :\n");
			// print_matrix_1_3(data->ray.scaled_vector);
			return (hit_result);
		}
		step += 15;
		// if (PRINT_DEBUG) print_matrix_1_3(data->ray.direction_abc);
	}
	return (((int)NADA));
}

void	send_rays(t_data *data)
{
	int 	pixel_x;
	int 	pixel_y;
	int		color;
	float 	start_angle_horiz = -data->camara.field_of_view_degrees / 2;
	float 	start_angle_vert = data->camara.field_of_view_degrees / 2;
	float 	angle_horiz;
	float 	angle_vert;

	float 	angle_step_horiz = data->camara.field_of_view_degrees / WINDOW_WIDTH;
	float 	angle_step_vert = data->camara.field_of_view_degrees / WINDOW_HEIGHT;

	angle_vert = start_angle_vert;
	pixel_x = 0;
	pixel_y = 0;
	while (pixel_y <= WINDOW_HEIGHT)
	{
		pixel_x = 0;
		angle_horiz = start_angle_horiz;
		while (pixel_x <= WINDOW_WIDTH)
		{
			color = hit_ray(data, angle_horiz, angle_vert);
			put_pixel_img(data->image, pixel_x, pixel_y, color);
			if (color != NADA)
			{
				// printf("angle hori verti: %f\t%f\n", angle_horiz, angle_vert);
				// printf("xy: %d\t%d\n", pixel_x, pixel_y);
			}
			pixel_x++;
			angle_horiz += angle_step_horiz;
		}
		// sleep(2);
		angle_vert -= angle_step_vert;
		pixel_y++;
	}
	printf("done\n");
}

int main(int argc, char **argv)
{
	t_data	data;

	data.mlx = new_window(WINDOW_HEIGHT, WINDOW_WIDTH, "My first window");
	if (!data.mlx.win_ptr)
		return (1);
	data.image = new_img(WINDOW_HEIGHT, WINDOW_WIDTH, data.mlx);
	mlx_key_hook(data.mlx.win_ptr, handle_input, &data);
	draw_square(&data, (int [2]){200, 200}, (int [2]){200, 200});

	init_camara(&data);
	init_sphere(&data);

	send_rays(&data);

	mlx_put_image_to_window(data.mlx.mlx_ptr, data.mlx.win_ptr, data.image.img_ptr, 0, 0);

	mlx_loop(data.mlx.mlx_ptr);
	mlx_destroy_window(data.mlx.mlx_ptr, data.mlx.win_ptr);
	destroy_image(data.image);
	free(data.mlx.mlx_ptr);
	(void)argc;
	(void)argv;
}

/**
 * calculator:
 * https://matrix.reshish.com/multCalculation.php
 * 
 * https://www.studysmarter.co.uk/explanations/math/geometry/3-dimensional-vectors/
 * https://math.libretexts.org/Bookshelves/Applied_Mathematics/Mathematics_for_Game_Developers_(Burzynski)/04%3A_Matrices/4.06%3A_Rotation_Matrices_in_3-Dimensions
*/