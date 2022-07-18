/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:56:41 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 10:03:11 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

double	get_ratio(int current, int start, int end)
{
	double	ratio;

	if (start == end)
		return (1.0);
	ratio = (double)(current - start) / (end - start);
	return (ratio);
}

static int	process_lerp(int start, int end, double ratio)
{
	return ((int)((ratio) * end + (1 - ratio) * start));
}

int	get_col(int isX, int current, t_point v1, t_point v2)
{
	double	ratio;
	int		red;
	int		green;
	int		blue;

	if (v1.color == v2.color)
		return (v1.color);
	if (isX)
		ratio = get_ratio(current, v1.screenx, v2.screenx);
	else
		ratio = get_ratio(current, v1.screeny, v2.screeny);
	red = process_lerp((v1.color >> 16) & 0xFF, (v2.color >> 16) & 0xFF, ratio);
	green = process_lerp((v1.color >> 8) & 0xFF, (v2.color >> 8) & 0xFF, ratio);
	blue = process_lerp((v1.color) & 0xFF, (v2.color) & 0xFF, ratio);
	return ((red << 16) | (green << 8) | blue);
}
