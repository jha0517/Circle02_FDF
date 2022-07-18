/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:54:06 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 09:57:05 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

float	get_origin_x(float x, float z, float r)
{
	return ((x - z) * cos(26.567 * M_PI / 180) * r);
}

float	get_origin_y(float x, float z, int height, float r)
{
	return (((x + z + 1) * sin(26.567 * M_PI / 180) - height) * r);
}

float	get_screen_x(int col, int row, float r)
{
	return ((col - row) * cos(26.567 * M_PI / 180) * r);
}

float	get_screen_y(int col, int row, int height, float r)
{
	return ((((col + row) * sin(26.567 * M_PI / 180)) * r) - height);
}
