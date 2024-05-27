/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:42:48 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/26 13:56:33 by rikverhoeve      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define M_PI           3.14159265358979323846
# define PRINT_DEBUG	1
# define WINDOW_HEIGHT 800
# define WINDOW_WIDTH 800
# define DEGR_10_IN_RAD 0.1745329252


#include <math.h>
#include <unistd.h>
#include <stdio.h>

enum keys
{
	ON_ESC = 65307
};

enum e_error_codes
{
	IN_BOUNDS,
	OUT_OF_BOUNDS
};

enum bool
{
	FALSE,
	TRUE
};

typedef struct s_win
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		height;
	int		width;
}		t_win;

enum e_hex_code
{
	NADA = 0x00000000,
	BLACK = 0x000000FF,
	WHITE = 0xFFFFFFFF,
	RED = 0x00FF00FF,
	GREEN = 0x0000FFFF,
	BLUE = 0x00000FFF,
	ORANGE = 0xFFA500
};

typedef struct s_img
{
	t_win	win;
	void	*img_ptr;
	char	*addr;
	int		h;
	int		w;
	int		bpp;
	int		endian;
	int		line_len;
}		t_img;

typedef struct s_cylinder_object
{
	int		object_center_xyz[3];
	float	axis_orientation_normalized_vectors[3];
	float	diameter;
	float	height;
	int		color;
} t_cylinder_object;

typedef struct s_sphere_object
{
	float	object_center_xyz[3];
	float	radius;
	float	height;
	int		color;
} t_sphere_object;

typedef struct s_ray
{
	float	direction_abc[3];
	float	vector_scalar_step;
	float	scaled_vector[3];
} t_ray;

typedef struct s_camara
{
	int		pos_xyz[3];
	float	view_orientation_matrix[3];
	float	field_of_view_rad;
} t_camara;

typedef struct s_data
{
	t_win				mlx;
	t_img				image;
	t_sphere_object		sphere;
	t_cylinder_object	cylinder;
	t_camara			camara;
	t_ray				ray;
} t_data;

typedef struct s_transform_index
{
	int	j;
	int	i;
	int	l;
	int	m;

}	t_transform_index;

enum e_key_codes
{
	UP = 65362,
	DOWN = 65364,
	LEFT = 65361,
	RIGHT = 65363
};

typedef struct s_ray_sending_tools
{
	int 	pixel_x;
	int 	pixel_y;
	float 	start_angle_horiz;
	float 	start_angle_vert;
	float 	angle_horiz;
	float 	angle_vert;
	int		half_screen_width;
	int		half_screen_height;
	float	perpendicular_distance_horiz_triangle;
	float	perpendicular_distance_vert_triangle;
} t_ray_sending_tools;

void	matrix_multiplication(float comp[3][3], t_ray *ray, float camara_vector[3]);
void	matrix_multiply_1x3_3x3(float m1[3], float m2[3][3], float result_m[3]);
void	compilation_matrix(float comp[3][3], float R[3][3], float R3[3][3]);
void	copy_matrix(float dst[3], float src[3]);

void	init_t_around_z(float R[3][3], float rad);
void	init_t_around_y(float R[3][3], float rad);
void	init_t_around_x(float R[3][3], float rad);
void	init_result(float M[3]);
double	ft_degr_to_rad(float x);
void	vector_scaling(float v[3], float scale);
void	send_rays(t_data *data);

#endif
