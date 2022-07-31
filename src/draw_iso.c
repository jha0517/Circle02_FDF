/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_iso.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:00:52 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/27 17:11:50 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include "../include/libft.h"

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
	free(start);
	return (p1);
}

void	calcule_screen_pos(t_point *p1, t_data *data)
{
	float	sx;
	float	so;
	float	sc;

	sx = get_screen_x(p1->col, p1->row, data->edgel);
	so = get_origin_x(data->col / 2.0, data->row / 2.0, data->edgel);
	sc = (data->win_w / 2 + data->offsetx);
	p1->screenx = sx - so + sc;
	sx = get_screen_y(p1->col, p1->row, p1->height * data->mul, data->edgel);
	so = get_origin_y(data->col / 2.0, data->row / 2.0, 1, data->edgel);
	sc = (data->win_h / 2 + data->offsety);
	p1->screeny = sx - so + sc;
}

void	convert_world_to_screen(int Addj, int i, int j, t_data *data)
{
	t_point	*p1;
	t_point	*p2;

	p1 = &data->ptab[i][j];
	if (Addj == 1)
		p2 = &data->ptab[i][j + 1];
	else
		p2 = &data->ptab[i + 1][j];
	calcule_screen_pos(p1, data);
	calcule_screen_pos(p2, data);
	draw_line(*p1, *p2, data);
}

void	draw_iso(t_data *img)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			if (j + 1 < img->col)
				convert_world_to_screen(1, i, j, img);
			if (i + 1 < img->row)
				convert_world_to_screen(0, i, j, img);
			j += 1;
		}
		i += 1;
	}
}

void	iterate_entire_iso(t_data *img)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			img->ptab[i][j] = set_point(i, img->tab[i][j], j);
			if (j + 1 < img->col)
			{	
				img->ptab[i][j + 1] = set_point(i, img->tab[i][j + 1], j + 1);
				convert_world_to_screen(1, i, j, img);
			}
			if (i + 1 < img->row)
			{
				img->ptab[i + 1][j] = set_point(i + 1, img->tab[i + 1][j], j);
				convert_world_to_screen(0, i, j, img);
			}
			j += 1;
		}
		i += 1;
	}
}
