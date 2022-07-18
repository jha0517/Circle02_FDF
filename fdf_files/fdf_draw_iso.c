/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw_iso.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:00:52 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 10:19:37 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_point	set_point(int x, char *str, int z)
{
	t_point	p1;
	char	*color_str;
	char	*start;

	p1.row = x;
	p1.col = z;
	start = ft_strdup(str);
	color_str = ft_strchr(start, ',');
	if (color_str)
	{
		*color_str = '\0';
		color_str += 3;
		p1.color = convert_from_str_hex_to_decimal(color_str);
	}
	else
	{
		p1.color = 0x00FFFFFF;
	}
	p1.height = ft_atoi(start);
	return (p1);
}

void	convert_world_to_screen(t_point p1, t_point p2, float r, t_data *data)
{
	int	xc;
	int	yc;

	xc = data->window_wigth / 2;
	yc = data->widow_height / 2;
	p1.screenx = get_screen_x(p1.col, p1.row, r) - get_origin_x(data->col / 2.0, data->row / 2.0, r) + xc;
	p1.screeny = get_screen_y(p1.col, p1.row, p1.height, r) - get_origin_y(data->col / 2.0, data->row / 2.0, 1, r) + yc;
	p2.screenx = get_screen_x(p2.col, p2.row, r) - get_origin_x(data->col / 2.0, data->row / 2.0, r) + xc;
	p2.screeny = get_screen_y(p2.col, p2.row, p2.height, r) - get_origin_y(data->col / 2.0, data->row / 2.0, 1, r) + yc;
	draw_line(p1, p2, data);
}

void	iterate_entire_iso(t_data *img, char ***tab)
{
	int		i;
	int		j;
	t_point	p1;
	t_point	p2;

	i = 0;
	j = 0;
	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			if (j + 1 < img->col)
			{	
				p1 = set_point(i, tab[i][j], j);
				p2 = set_point(i, tab[i][j + 1], j + 1);
				convert_world_to_screen(p1, p2, img->edgel, img);
			}
			if (i + 1 < img->row)
			{
				p1 = set_point(i, tab[i][j], j);
				p2 = set_point(i + 1, tab[i + 1][j], j);
				convert_world_to_screen(p1, p2, img->edgel, img);
			}
			j += 1;
		}
		i += 1;
	}
}
