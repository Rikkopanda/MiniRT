/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rikverhoeven <rikverhoeven@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:42:48 by rikverhoeve       #+#    #+#             */
/*   Updated: 2024/05/28 20:35:19 by rikverhoeve      ###   ########.fr       */
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
#include "../minilibx-linux/mlx.h"
#include <stdlib.h>

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
	NADA = -1,
	BLACK = 0x000000FF,
	GREY = 0x808080,
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

typedef struct s_vec
{
	float m[3];
} t_vec;

typedef struct s_light_source
{
	float		position[3];
	int		rgb[3];
	float	brightness; // range 0.0-1.0
} t_light_source;

/*
Candle 255, 147, 41

40W Tungsten 255, 197, 143

100W Tungsten 255, 214, 170

Halogen 255, 241, 224

Carbon Arc 255, 250, 244

High Noon Sun 255, 255, 251

Direct Sunlight 255, 255, 255

Overcast Sky 201, 226, 255

Clear Blue Sky 64, 156, 255

Warm Fluorescent 255, 244, 229

Standard Fluorescent 244, 255, 250

Cool White Fluorescent 212, 235, 255

Full Spectrum Fluorescent 255, 244, 242

Grow Light Fluorescent 255, 239, 247

Black Light Fluorescent 167, 0, 255

Mercury Vapor 216, 247, 255

Sodium Vapor 255, 209, 178

Metal Halide 242, 252, 255

High Pressure Sodium 255, 183, 7
*/
typedef struct s_ambient_lightning //what does this mean, just reflected light?
{
	
	int		rgb[3]; // https://www.reddit.com/r/spaceengineers/comments/3e0k38/rgb_values_for_various_types_of_realworld_lights/
	float	lightning_ratio; // range 0.0-1.0
} t_ambient_lightning;

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
	t_vec	normalized_vec;
	t_vec	scaled_vec;
	float	world_pos_of_scaled_vec[3];
	float	step;
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
	t_light_source		light;
} t_data;

typedef struct s_transform_index
{
	int	j;
	int	i;
	int	l;
	int	m;
}	t_transform_index;

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

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

void	normalize_vector(float v[3]);
void	vector_scaling(float v[3], float scale);


int		interpolate(int color_A, int color_B, float t);

void	init_t_around_z(float R[3][3], float rad);
void	init_t_around_y(float R[3][3], float rad);
void	init_t_around_x(float R[3][3], float rad);
void	init_result(float M[3]);
float	ft_degr_to_rad(float x);
float	ft_rad_to_degr(float x);
void	send_rays(t_data *data);

void	put_pixel_img(t_img img, int x, int y, int color);
void	init_rgb(t_color *rgb, int color);

void	print_matrix_3_3(float m[3][3]);
void	print_matrix_1_3(float m[3]);

int		handle_input(int keysym, t_data *data);

t_win	new_window(int w, int h, char *str);
t_img	new_img(int w, int h, t_win window);
void	destroy_image(t_img img);

#endif
