/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:47:09 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/27 17:09:58 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include	"../include/get_next_line.h"
#include	"../include/libft.h"

static int	count_split(const char *str, char charset)
{
	int	counter;
	int	i;

	i = 0;
	counter = 0;
	while (str[i] && str[i] == charset)
		i++;
	while (str[i])
	{
		counter++;
		while (str[i] != charset && str[i])
			i++;
		while (str[i] == charset && str[i])
			i++;
	}
	return (counter);
}

char	**put_in_tab(char **tab, char *line, int totalNumData)
{
	int		i;

	i = 0;
	tab = ft_split(line, ' ');
	while (tab[i])
		i++;
	if (i != totalNumData)
		return (NULL);
	return (tab);
}

void	create_ptab(t_data *img)
{
	int	i;

	i = 0;
	img->ptab = (t_point **)malloc(sizeof(t_point *) * (img->row + 1));
	if (!img->ptab)
		return ;
	img->ptab[img->row] = 0;
	while (i < img->row)
	{
		img->ptab[i] = (t_point *)malloc(sizeof(t_point) * (img->col + 1));
		if (!img->ptab[i])
			return ;
		i++;
	}
}

int	read_and_write(int fd, t_data *img)
{
	char	*line;
	int		t;
	int		col;

	line = "a";
	while (line)
	{
		line = get_next_line(fd);
		if (line)
		{	
			img->row++;
			t = img->row - 1;
			img->col = count_split(line, ' ');
			if (t == 0)
				col = img->col;
			if (col != img->col)
				return (0);
			img->tab[t] = put_in_tab(img->tab[t], line, col);
			if (!img->tab[t])
				return (0);
			col = img->col;
			free(line);
		}
	}
	return (1);
}

char	***parsing_fdf(char *file_name, t_data *img)
{
	int		i;
	int		j;
	int		fd;

	fd = create_tab(file_name, img);
	if (!fd)
		return (NULL);
	img->row = 0;
	i = read_and_write(fd, img);
	if (!i)
		return (NULL);
	j = (img->row + img->col - 2);
	img->edgel = (img->win_w / cos(26.567 * M_PI / 180) / j) * 0.9;
	if (img->edgel == 0)
		return (NULL);
	create_ptab(img);
	close(fd);
	return (img->tab);
}
