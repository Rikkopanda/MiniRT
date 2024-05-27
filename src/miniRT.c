/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rverhoev <rverhoev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:06:23 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/27 09:52:45 by rverhoev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "stdio.h"
#include <stdlib.h>
#include "../include/miniRT.h"

void	init_sphere(t_data *data)
{
	data->sphere.radius = 10;
	data->sphere.color = ORANGE;
	data->sphere.object_center_xyz[0] = 50;
	data->sphere.object_center_xyz[1] = 0;
	data->sphere.object_center_xyz[2] = 0;
}

void	init_camara(t_data *data)
{
	data->camara.field_of_view_rad = ft_degr_to_rad(150);
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