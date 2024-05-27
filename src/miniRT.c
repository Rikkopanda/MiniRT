/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rverhoev <rverhoev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:06:23 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/27 08:51:54 by rverhoev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	data->sphere.object_center_xyz[0] = 100;
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