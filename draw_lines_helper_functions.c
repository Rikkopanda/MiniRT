/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_lines_helper_functions.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 20:34:45 by rik               #+#    #+#             */
/*   Updated: 2024/05/12 19:24:26 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	interpolate(t_data *data, t_point	*ori, t_point	*s, int count)
{
	int		color;
	t_point	new_color;
	double	percentage;

	percentage = (double)count / (double)data->dr_data.curr_line_steps;
	new_color.r = (int)(((double)ori->r * ((double)1 - percentage))
			+ (double)s->r * percentage);
	new_color.g = (int)(((double)ori->g * ((double)1 - percentage))
			+ (double)s->g * percentage);
	new_color.b = (int)(((double)ori->b * ((double)1 - percentage))
			+ (double)s->b * percentage);
	new_color.a = (int)(((double)ori->a * ((double)1 - percentage))
			+ (double)s->a * percentage);
	color = ft_pixel_rgba(new_color.r, new_color.g, new_color.b, new_color.a);
	return (color);
}
// printf("\ninterpolated at %f%%\nold rgba: %d, %d, %d, %d\nnew rgba:
// %d, %d, %d, %d\n", percentage * 100, ori->r, ori->g, ori->b, ori->b,
// new_color.r, new_color.g, new_color.b, new_color.a);
// printf("t_point A %x to B %x, interpolated result: %x\n\n",
// ori->color, s->color, color);

void	calc_line_len(t_data *data, t_point A_cnt, t_point B_cnt)
{
	data->dr_data.curr_line_steps = 0;
	while (A_cnt.d_xy[0] != B_cnt.d_xy[0] || A_cnt.d_xy[1] != B_cnt.d_xy[1])
	{
		data->dr_data.error[1] = data->dr_data.error[0] * 2;
		if (data->dr_data.error[1] > -data->dr_data.delta.d_xy[1])
		{
			data->dr_data.error[0] -= data->dr_data.delta.d_xy[1];
			A_cnt.d_xy[0] += data->dr_data.sign_dir.d_xy[0];
		}
		if (data->dr_data.error[1] < data->dr_data.delta.d_xy[0])
		{
			data->dr_data.error[0] += data->dr_data.delta.d_xy[0];
			A_cnt.d_xy[1] += data->dr_data.sign_dir.d_xy[1];
		}
		data->dr_data.curr_line_steps++;
	}
}

void	init_draw_data(t_data *data, t_point *f, t_point *s)
{
	data->dr_data.delta.d_xy[0] = ft_abs(s->d_xy[0] - f->d_xy[0]);
	data->dr_data.delta.d_xy[1] = ft_abs(s->d_xy[1] - f->d_xy[1]);
	if (f->d_xy[0] < s->d_xy[0])
		data->dr_data.sign_dir.d_xy[0] = 1;
	else
		data->dr_data.sign_dir.d_xy[0] = -1;
	if (f->d_xy[1] < s->d_xy[1])
		data->dr_data.sign_dir.d_xy[1] = 1;
	else
		data->dr_data.sign_dir.d_xy[1] = -1;
	data->dr_data.error[0] = data->dr_data.delta.d_xy[0]
		- data->dr_data.delta.d_xy[1];
	data->dr_data.error[1] = 0;
	data->dr_data.cur = *f;
	data->dr_data.draw_color = (int)white;
}

int	check_img_bounds(t_data *data, int dx, int dy)
{
	int	dx_offset;
	int	dy_offset;

	dx_offset = data->map.world_pos_d_xy[0];
	dy_offset = data->map.world_pos_d_xy[1];
	if ((dx + dx_offset >= 0 && dy + dy_offset >= 0)
		&& (dx + dx_offset <= WIDTH && dy + dy_offset <= HEIGHT))
		return (0);
	return (1);
}
