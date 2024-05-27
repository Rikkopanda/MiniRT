/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rverhoev <rverhoev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 15:29:32 by rverhoev          #+#    #+#             */
/*   Updated: 2024/05/27 10:05:57 by rverhoev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	init_comp_m(float comp[3][3])
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			comp[i][j] = 0;
			j++;
		}
		i++;
	}
}

void	compilation_matrix(float comp[3][3], float R[3][3], float R3[3][3])
{
	int	i;
	int	j;
	int	k;

	i = 0;
	init_comp_m(comp);
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			k = 0;
			while (k < 3)
			{
				comp[i][j] += R[i][k] * R3[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
}

void	matrix_multiplication(float comp[3][3], t_ray *ray, float camara_vector[3])
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			ray->direction_abc[i]
				+= camara_vector[j]
				* comp[j][i];
			j++;
		}
		i++;
	}
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

void	matrix_multiply_1x3_3x3(float m1[3], float m2[3][3], float result_m[3])
{
	int	i;
	int	j;

	init_result(result_m);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < 3)
		{
			result_m[i]
				+= m1[j]
				* m2[j][i];
			j++;
		}
		i++;
	}
}


void	vector_scaling(float v[3], float scale)
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		v[i]
			= v[i] * scale;
		i++;
	}
}

float	ft_rad_to_degr(float x)
{
	if (x == 0)
		return (0);
	x = (x / (2 * M_PI)) * (float)360;
	return (x);
}

float	ft_degr_to_rad(float x)
{
	if (x == 0)
		return (0);
	x = (x * M_PI) / (float)180;
	return (x);
}

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