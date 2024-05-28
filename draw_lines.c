/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_lines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:30:07 by rverhoev          #+#    #+#             */
/*   Updated: 2024/05/13 20:39:05 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	draw_line(t_data *data, t_point *f, t_point *s);

int		check_img_bounds(t_data *data, int Dx, int Dy);

void	draw_lines_init(t_point **b, t_point **c, int *i)
{
	*c = NULL;
	*b = NULL;
	*i = 0;
	(void)b;
	(void)c;
}

void	draw_lines(t_data *data)
{
	t_draw_line_vars	var;

	draw_lines_init(&var.b, &var.c, &var.i);
	while (var.i < data->map.max_height)
	{
		var.j = 0;
		while (var.j < data->map.max_width
			&& data->map.coords[var.i][var.j].point_assigned_bool)
		{
			var.origin = (t_point *)&data->map.coords[var.i][var.j];
			if (var.j < data->map.max_width - 1)
				var.b = (t_point *)&data->map.coords[var.i][var.j + 1];
			if (var.i < data->map.max_height - 1)
				var.c = (t_point *)&data->map.coords[var.i + 1][var.j];
			if (var.j < data->map.max_width - 1
				&& data->map.coords[var.i][var.j + 1].point_assigned_bool)
				draw_line(data, var.origin, var.b);
			if (var.i < data->map.max_height - 1
				&& data->map.coords[var.i + 1][var.j].point_assigned_bool)
				draw_line(data, var.origin, var.c);
			var.j++;
		}
		var.i++;
	}
}
// printf("aaa nbr x %d \n", data->map.coords[0][2].d_xy[0]);
// printf("aaa nbr y %d \n", data->map.coords[0][2].d_xy[1]);
//draw_xy_projection(data);
// printf("rgba: %d, %d, %d, %d\n", (*origin).r,
// (*origin).g, (*origin).b, (*origin).a);
// printf("rgba: %d, %d, %d, %d\n\n", (*c).r, (*c).g, (*c).b, (*c).a);
//assign_vectors_len(data, i , j);

void	move_cur_draw_xy(t_data *data, t_point *cur)
{
	data->dr_data.error[1] = data->dr_data.error[0];
	if ((data->dr_data.error[1] * 2)
		> -data->dr_data.delta.d_xy[1])
	{
		data->dr_data.error[0] -= data->dr_data.delta.d_xy[1];
		(*cur).d_xy[0] += data->dr_data.sign_dir.d_xy[0];
	}
	if (data->dr_data.error[1] < data->dr_data.delta.d_xy[0])
	{
		data->dr_data.error[0] += data->dr_data.delta.d_xy[0];
		(*cur).d_xy[1] += data->dr_data.sign_dir.d_xy[1];
	}
}

void	bresinham_algorithme(t_data *data, t_point *f, t_point *s, int count)
{
	t_point				cur;

	cur = data->dr_data.cur;
	while (cur.d_xy[0] != (*s).d_xy[0]
		|| cur.d_xy[1] != (*s).d_xy[1])
	{
		if (cur.color != (*s).color)
			data->dr_data.draw_color = interpolate(data, f, s, count);
		else
			data->dr_data.draw_color = cur.color;
		if (check_img_bounds(data, cur.d_xy[0],
				cur.d_xy[1]) == 0)
			put_pixel_img(data->image, cur.d_xy[0]
				+ data->map.world_pos_d_xy[0], cur.d_xy[1]
				+ data->map.world_pos_d_xy[1], data->dr_data.draw_color);
		move_cur_draw_xy(data, &cur);
		count++;
	}
}

void	draw_line(t_data *data, t_point *f, t_point *s)
{
	int	count;

	init_draw_data(data, f, s);
	calc_line_len(data, *f, *s);
	count = 0;
	bresinham_algorithme(data, f, s, count);
}
// fdf_put_pixel(data, data->dr_data.cur.d_xy[0], data->dr_data.cur.d_xy[0], 1);
//printf("error[0] = %d [1] = %d\n", error[1], error[0]);
//printf("total count %d, D_len  \n", count);
//printf("deltax = %d y = %d\n", delta.d_xy[0], delta.d_xy[1]);
