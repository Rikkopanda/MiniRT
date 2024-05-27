/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:06:23 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/26 14:02:18 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minilibx-linux/mlx.h"
#include "stddef.h"
#include "stdio.h"
#include <stdlib.h>
#include "../include/miniRT.h"

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


int	extra_keys(int keysym, t_data *data)
{
	if (keysym == ON_ESC)
		return (mlx_loop_end(data->mlx.mlx_ptr), -1);
	else
		return (0);
}

int	left_up_right_down_forward_backward(int keysym, t_data *data)
{
	if (keysym == 'w')
		return (data->camara.pos_xyz[0] += 10, 1);
	else if (keysym == 's')
		return (data->camara.pos_xyz[0] -= 10, 1);	
	else if (keysym == 'a')
		return (data->camara.pos_xyz[1] += 10, 1);	
	else if (keysym == 'd')
		return (data->camara.pos_xyz[1] -= 10, 1);	
	else if (keysym == 'r')
		return (data->camara.pos_xyz[2] += 10, 1);	
	else if (keysym == 'f')
		return (data->camara.pos_xyz[2] -= 10, 1);
	else
		return (0);
}

// int	rotate_around_z(int keysym, t_data *data)
// {
// }
// int	rotate_around_y(int keysym, t_data *data)
// {
// }

int	rotate_view(int keysym, t_data *data)
{
	float	add_angle[3][3];
	float	original_orientation_matrix[3];

	copy_matrix(original_orientation_matrix, data->camara.view_orientation_matrix);
	if (keysym == UP)
	{
		init_t_around_y(add_angle, DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);	
	}
	else if (keysym == DOWN)
	{
		init_t_around_y(add_angle, -DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);		
	}
	else if (keysym == LEFT)
	{
		init_t_around_z(add_angle, DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);		
	}
	else if (keysym == RIGHT)
	{
		init_t_around_z(add_angle, -DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);	
	}
	else
		return (0);
}
/**
 * @note putting all into a while() to select function pointer could be faster than if else's?
 * 
*/
int handle_input(int keysym, t_data *data)
{
	if (left_up_right_down_forward_backward(keysym, data) == 0
		&& rotate_view(keysym, data) == 0)
		return (0);
	else if (extra_keys(keysym, data) < 0)
		return (0);
	send_rays(data);
	mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.win_ptr, data->image.img_ptr, 0, 0);
	printf("%d key\n", keysym);
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
	data->sphere.color = ORANGE;
	data->sphere.object_center_xyz[0] = 200;
	data->sphere.object_center_xyz[1] = 0;
	data->sphere.object_center_xyz[2] = 0;
}

void	init_camara(t_data *data)
{
	data->camara.field_of_view_rad = ft_degr_to_rad(70);
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

/**
 * https://www.freetext.org/Introduction_to_Linear_Algebra/Basic_Vector_Operations/Normalization/
*/
void normalize_vector(float v[3])
{
	float magnitude;
	double squared;

	squared = pow((double)v[0], 2) + pow((double)v[1], 2) + pow((double)v[2], 2);
	magnitude = (float)sqrt(squared);
	// printf("before\n");
	// print_matrix_1_3(v);
	vector_scaling(v, 1 / magnitude);
	// printf("after\n");
	// print_matrix_1_3(v);
	// return ()
}

void set_rgb_factor(float *rgb_factor)
{
	*rgb_factor += 1;
	*rgb_factor *= 0.5;
}

int	interpolate(int color_A, int color_B, float t);
int create_color(int r, int g, int b);

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

int	get_r(int color)
{
	return (color >> 16 & 0xFF);
}

int	get_g(int color)
{
	return (color >> 8 & 0xFF);
}

int	get_b(int color)
{
	return (color & 0xFF);
}

void init_rgb(t_color *rgb, int color)
{
	rgb->r = get_r(color);
	rgb->g = get_g(color);
	rgb->b = get_b(color);
}

int create_color(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	interpolate(int color_A, int color_B, float t)
{
	t_color rgb_a;
	t_color rgb_b;
	t_color rgb_interpolate;


	init_rgb(&rgb_a, color_A);
	init_rgb(&rgb_b, color_B);

	rgb_interpolate.r = rgb_a.r + (rgb_b.r - rgb_a.r) * t;
	rgb_interpolate.g = rgb_a.g + (rgb_b.g - rgb_a.g) * t;
	rgb_interpolate.b = rgb_a.b + (rgb_b.b - rgb_a.b) * t;
	return (create_color(rgb_interpolate.r, rgb_interpolate.g, rgb_interpolate.b));
}

/**
 * @note optimisation ideas:
 * static or inline funtions
 * 
 * 
*/
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