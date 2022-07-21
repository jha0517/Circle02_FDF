/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw_iso.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:00:52 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/19 06:33:14 by hyunahjung       ###   ########.fr       */
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

void	convert_world_to_screen(int Addj, int i, int j, t_data *data)
{
	int		xc;
	int		yc;
	t_point *p1;
	t_point *p2;

	p1 = &data->ptab[i][j];
	if (Addj == 1)
		p2 = &data->ptab[i][j + 1];
	else
		p2 = &data->ptab[i + 1][j];
	xc = data->window_wigth / 2;
	yc = data->widow_height / 2;
	p1->screenx = get_screen_x(p1->col, p1->row, data->edgel) - get_origin_x(data->col / 2.0, data->row / 2.0, data->edgel) + xc;
	p1->screeny = get_screen_y(p1->col, p1->row, p1->height, data->edgel) - get_origin_y(data->col / 2.0, data->row / 2.0, 1, data->edgel) + yc;
	p2->screenx = get_screen_x(p2->col, p2->row, data->edgel) - get_origin_x(data->col / 2.0, data->row / 2.0, data->edgel) + xc;
	p2->screeny = get_screen_y(p2->col, p2->row, p2->height, data->edgel) - get_origin_y(data->col / 2.0, data->row / 2.0, 1, data->edgel) + yc;
	draw_line(*p1, *p2, data);
}

void	draw_iso(t_data *img)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	printf("draw iso.\n");
	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			if (j + 1 < img->col)
			{
				printf("%i, %i\n", i, j + 1);
				printf("scr.x : %i, scr.y : %i\n", img->ptab[i][j].screenx, img->ptab[i][j + 1].screeny);
				draw_line(img->ptab[i][j], img->ptab[i][j + 1], img);
			}
			if (i + 1 < img->row)
			{
				printf("%i, %i\n", i + 1, j);
				printf("scr.x : %i, scr.y : %i\n", img->ptab[i][j].screenx, img->ptab[i + 1][j].screeny);
				draw_line(img->ptab[i][j], img->ptab[i + 1][j], img);
			}
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
	img->ptab = (t_point **)malloc(sizeof(t_point *) * img->row + 1);
	if (!img->ptab)
		return ;
	while (i < img->row)
	{
		img->ptab[i] = (t_point *)malloc(sizeof(t_point) * img->col + 1);
		if (!img->ptab[i])
			return ;
		i++;
	}
	i = 0;
	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			if (j + 1 < img->col)
			{	
				img->ptab[i][j] = set_point(i, img->tab[i][j], j);
				printf("%i\n",img->ptab[i][j].col);
				img->ptab[i][j + 1] = set_point(i, img->tab[i][j + 1], j + 1);
				convert_world_to_screen(1, i, j, img);
				// convert_world_to_screen(img->ptab[i][j], img->ptab[i][j + 1], img);
			}
			if (i + 1 < img->row)
			{
				img->ptab[i][j] = set_point(i, img->tab[i][j], j);
				img->ptab[i + 1][j] = set_point(i + 1, img->tab[i + 1][j], j);
				convert_world_to_screen(0, i, j, img);
			}
			j += 1;
		}
		i += 1;
	}
}
