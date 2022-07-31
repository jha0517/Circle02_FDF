/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 13:20:52 by hyujung           #+#    #+#             */
/*   Updated: 2022/07/30 14:09:03 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include	"../include/get_next_line.h"
#include	"../include/libft.h"

int	count_nl(char *file_name)
{
	int		counter;
	char	*line;
	int		fd;

	counter = -1;
	fd = open(file_name, O_RDONLY);
	line = "a";
	while (line)
	{
		counter += 1;
		line = get_next_line(fd);
		free(line);
	}
	close(fd);
	free(line);
	return (counter);
}

int	create_tab(char *file_name, t_data *img)
{
	int	fd;

	img->row = count_nl(file_name);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (0);
	img->tab = (char ***)malloc((img->row + 1) * sizeof(char **));
	if (!img->tab)
		return (0);
	img->tab[img->row] = NULL;
	return (fd);
}
