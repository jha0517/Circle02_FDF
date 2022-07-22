/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:00:04 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/22 06:03:20 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, t_point v1, int color)
{
	char	*offset;

	// printf("pixel put\n");
	offset = data->addr + ((v1.y + data->offsety) * data->linel + (v1.x + data->offsetx) * (data->bpp / 8));
	*(unsigned int *)offset = color;
}

void	draw_line(t_point v1, t_point v2, t_data *data)
{
	int		dx;
	int		dy;
	float	p;

	v1.x = v1.screenx;
	v1.y = v1.screeny;
	dx = v2.screenx - v1.screenx;
	dy = v2.screeny - v1.screeny;
	if (dx < 0)
	{
			v1.xvalue = -1;
			dx *= -1;
	}
	else
		v1.xvalue = 1;
	if (dy < 0)
	{
		v1.yvalue = -1;
		dy *= -1;
	}
	else
		v1.yvalue = 1;
	p = 2 * dy - dx;
	if (dx > dy)
	{
		while (v1.x != v2.screenx)
		{
			if (v1.x > 0 && v1.y > 0)
				my_mlx_pixel_put(data, v1, get_col(1, v1.x, v1, v2));
			v1.x += v1.xvalue;
			if (p < 0)
				p = p + (2 * dy);
			else
			{
				p = p + (2 * dy) - (2 * dx);
				v1.y += v1.yvalue;
			}
		}
	}
	else
	{
		while (v1.y != v2.screeny)
		{
			if (v1.x > 0 && v1.y > 0)
				my_mlx_pixel_put(data, v1, get_col(0, v1.y, v1, v2));
			v1.y += v1.yvalue;
			if (p < 0)
				p = p + (2 * dx);
			else
			{
				p = p + (2 * dx) - (2 * dy);
				v1.x += v1.xvalue;
			}
		}
	}
}

int	find_inbase(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (i);
		i++;
	}
	return (0);
}

int	convert_from_str_hex_to_decimal(char *str)
{
	int	i;
	int	sum;
	int	len;

	i = 0;
	sum = 0;
	len = ft_strlen(str)-1;
	while (str[i] && 0 <= len)
	{
		sum += ft_recursive_power(16, i) * find_inbase(str[len], "0123456789ABCDEF");
		i++;
		len--;
	}
	return (sum);
}
