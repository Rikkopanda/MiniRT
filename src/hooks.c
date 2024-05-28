/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rverhoev <rverhoev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 09:26:30 by rverhoev          #+#    #+#             */
/*   Updated: 2024/05/28 10:59:48 by rverhoev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	extra_keys(int keysym, t_data *data)
{
	if (keysym == ON_ESC)
		return (mlx_loop_end(data->mlx.mlx_ptr), -1);
	else
		return (FALSE);
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

int move_lighting(int keysym, t_data *data)
{
	if (keysym == 'j')
		return (data->light.position[0] += 50, 1);
	else if (keysym == 'l')
		return (data->light.position[0] -= 50, 1);	
	else if (keysym == 'k')
		return (data->light.position[1] += 50, 1);	
	else if (keysym == 'i')
		return (data->light.position[1] -= 50, 1);	
	else if (keysym == 'o')
		return (data->light.position[2] += 50, 1);	
	else if (keysym == 'p')
		return (data->light.position[2] -= 50, 1);
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
	printf("rotating...\n");

	if (keysym == UP)
	{
		init_t_around_y(add_angle, DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);	
		return (TRUE);
	}
	else if (keysym == DOWN)
	{
		init_t_around_y(add_angle, -DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);
		return (TRUE);
	}
	else if (keysym == LEFT)
	{
		init_t_around_z(add_angle, DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);
		return (TRUE);

	}
	else if (keysym == RIGHT)
	{
		init_t_around_z(add_angle, -DEGR_10_IN_RAD);
		matrix_multiply_1x3_3x3(original_orientation_matrix, add_angle, data->camara.view_orientation_matrix);
		return (TRUE);
	}
	return (FALSE);
}

/**
 * @note putting all into a while() to select function pointer could be faster than if else's?
 * 
*/
int handle_input(int keysym, t_data *data)
{
	if (left_up_right_down_forward_backward(keysym, data) == FALSE
		&& rotate_view(keysym, data) == FALSE
		&& extra_keys(keysym, data) == FALSE
		&& move_lighting(keysym, data) == FALSE)
		return (0);
	send_rays(data);
	mlx_put_image_to_window(data->mlx.mlx_ptr, data->mlx.win_ptr, data->image.img_ptr, 0, 0);
	printf("%d key\n", keysym);
    return (0);
}