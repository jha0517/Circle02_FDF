/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:47:09 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 13:07:45 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	char	*copy;
	char	*start_pos;
	int		count_data;

	copy = ft_strdup(line);
	count_data = 0;
	while (*copy)
	{
		while (*copy && *copy == ' ')
			copy++;
		start_pos = copy;
		while (*copy && *copy != ' ')
			copy++;
		*copy = '\0';
		tab[count_data] = start_pos;
		count_data++;
		if (count_data < totalNumData)
			copy++;
	}
	return (tab);
}

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
	}
	close(fd);
	return (counter);
}

char	***parsing_fdf(char ***tab, char *file_name, t_data *img)
{
	char	*line;
	int		fd;
	int		i;
	int		j;

	i = 0;
	img->row = count_nl(file_name);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	tab = (char ***)malloc(img->row * sizeof(char **));
	if (!tab)
		return (NULL);
	img->row = 0;
	line = "a";
	while (line)
	{
		line = get_next_line(fd);
		if (line)
		{	
			img->row++;
			img->col = count_split(line, ' ');
			tab[img->row - 1] = (char **)malloc(img->col * sizeof(char *));
			if (!tab[img->row - 1])
				return (NULL);
			tab[img->row - 1] = put_in_tab(tab[img->row - 1], line, img->col);
			j = 0;
		}
	}
	img->edgel = (img->window_wigth / cos(26.567 * M_PI / 180) / (img->row + img->col - 2)) * 0.8;
	if (img->edgel == 0)
		return (NULL);
	close(fd);
	free(line);
	return (tab);
}
