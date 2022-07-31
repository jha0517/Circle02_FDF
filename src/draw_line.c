/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:00:04 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/26 11:21:54 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include "../include/libft.h"

void	my_mlx_pixel_put(t_data *data, t_point v1, int color)
{
	char	*offset;

	offset = data->addr + (v1.y * data->linel + v1.x * (data->bpp / 8));
	*(unsigned int *)offset = color;
}

int	check_if_in_window(t_data *data, t_point *v1)
{
	if (v1->x <= 0 || v1->y <= 0)
		return (0);
	else if (v1->x >= data->win_w || v1->y >= data->win_h)
		return (0);
	else
		return (1);
}

void	draw_line_1(t_point *v1, t_point *v2, t_data *data)
{
	while (v1->x != v2->screenx)
	{
		if (check_if_in_window(data, v1))
			my_mlx_pixel_put(data, *v1, get_col(1, v1->x, *v1, *v2));
		v1->x += v1->xvalue;
		if (data->p < 0)
			data->p = data->p + (2 * data->dy);
		else
		{
			data->p = data->p + (2 * data->dy) - (2 * data->dx);
			v1->y += v1->yvalue;
		}
	}
}

void	draw_line_2(t_point *v1, t_point *v2, t_data *data)
{
	while (v1->y != v2->screeny)
	{
		if (check_if_in_window(data, v1))
			my_mlx_pixel_put(data, *v1, get_col(0, v1->y, *v1, *v2));
		v1->y += v1->yvalue;
		if (data->p < 0)
			data->p = data->p + (2 * data->dx);
		else
		{
			data->p = data->p + (2 * data->dx) - (2 * data->dy);
			v1->x += v1->xvalue;
		}
	}
}

void	draw_line(t_point v1, t_point v2, t_data *data)
{
	v1.x = v1.screenx;
	v1.y = v1.screeny;
	data->dx = v2.screenx - v1.screenx;
	data->dy = v2.screeny - v1.screeny;
	if (data->dx < 0)
	{
			v1.xvalue = -1;
			data->dx *= -1;
	}
	else
		v1.xvalue = 1;
	if (data->dy < 0)
	{
		v1.yvalue = -1;
		data->dy *= -1;
	}
	else
		v1.yvalue = 1;
	data->p = 2 * data->dy - data->dx;
	if (data->dx > data->dy)
		draw_line_1(&v1, &v2, data);
	else
		draw_line_2(&v1, &v2, data);
	return ;
}
