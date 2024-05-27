
#include "miniRT.h"

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
