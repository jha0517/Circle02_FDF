/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:52:47 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/17 15:13:06 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "mlx/mlx.h"
#include <fcntl.h>
#include "get_next_line/get_next_line.h"
#include "libft/libft.h"
#define KEY_ESC 53

typedef struct s_data{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img;
	int		window_wigth;
	int		widow_height;
	char	*addr;
	int		bits_per_pixel;
	int		linel;
	int		endian;
	float	edgel;
	int		row;
	int		col;
}	t_data;

typedef struct s_point{
	int	row;
	int	height;
	int	col;
	int	screenx;
	int	screeny;
	int	color;
}	t_point;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*offset;

	offset = data->addr + (y * data->linel + x * (data->bits_per_pixel / 8));
	*(unsigned int *)offset = color;
}

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

int	get_current_color(int current, int start, int end, int fromcol, int tocol)
{
	double	ratio;
	int		red;
	int		green;
	int		blue;

	if (fromcol == tocol)
		return (fromcol);
	ratio = get_ratio(current, start, end);
	red = process_lerp((fromcol >> 16) & 0xFF, (tocol >> 16) & 0xFF, ratio);
	green = process_lerp((fromcol >> 8) & 0xFF, (tocol >> 8) & 0xFF, ratio);
	blue = process_lerp((fromcol) & 0xFF, (tocol) & 0xFF, ratio);

	return ((red << 16) | (green << 8) | blue);
}

void draw_line(int x1, int y1, int x2, int y2, t_data *data, int fromcolor, int tocolor)
{
	int	x = x1;
	int	y = y1;
	int dx;
	int	dy;
	float p;
	int xvalue;
	int yvalue;
	int	color;

	dx = x2-x1;
	dy = y2-y1;
	if (dx < 0)
	{
			xvalue = -1;
			dx *= -1;
	}
	else
		xvalue = 1;
	if (dy < 0)
	{
		yvalue = -1;
		dy *= -1;
	}
	else
		yvalue = 1;
	p = 2 * dy - dx;
	if (dx > dy)
	{
		while (x != x2)
		{
			color = get_current_color(x, x1, x2, fromcolor, tocolor);
			my_mlx_pixel_put(data, x, y, color);		
			x+= xvalue;
			if (p < 0)
				p = p + (2 * dy);
			else
			{
				p = p + (2 * dy) - (2 * dx);
				y+= yvalue;
			}
		}
	}
	else
	{
		while (y != y2)
		{
			color = get_current_color(y, y1, y2, fromcolor, tocolor);
			my_mlx_pixel_put(data, x, y, color);		
			y+= yvalue;
			if (p < 0)
				p = p + (2 * dx);
			else
			{
				p = p + (2 * dx) - (2 * dy);
				x+= xvalue;
			}
		}
	}
}

float get_origin_x(float x, float z, float r)
{
	return (x - z) * cos(26.567 * M_PI / 180) * r;
}

float get_origin_y(float x, float z, int height, float r)
{
	return (((x + z + 1) * sin(26.567 * M_PI / 180) - height) * r);
}

float get_screen_x(int col, int row, float r)
{
	return (col - row) * cos(26.567 * M_PI / 180) * r;
}

float get_screen_y(int col, int row, int height, float r)
{
	return (((col + row) * sin(26.567 * M_PI / 180)) * r) - height;
}

void convert_world_to_screen(t_point p1, t_point p2,float r, t_data *data)
{
	int xc;
	int yc;

	xc = data->window_wigth / 2;
	yc = data->widow_height / 2;
	p1.screenx = get_screen_x(p1.col, p1.row, r) - get_origin_x(data->col/2.0, data->row/2.0, r) + xc;
	p1.screeny = get_screen_y(p1.col, p1.row, p1.height, r) - get_origin_y(data->col/2.0, data->row/2.0, 1, r) + yc;
	p2.screenx = get_screen_x(p2.col, p2.row, r) - get_origin_x(data->col/2.0, data->row/2.0, r) + xc;
	p2.screeny = get_screen_y(p2.col, p2.row, p2.height, r) - get_origin_y(data->col/2.0, data->row/2.0, 1, r) + yc;
	draw_line(p1.screenx, p1.screeny, p2.screenx,p2.screeny, data, p1.color, p2.color);
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
		color_str+=3;
		p1.color = convert_from_str_hex_to_decimal(color_str);
	}
	else
	{
		p1.color = 0x00FFFFFF;
	}
	p1.height = ft_atoi(start);
	return p1;
}

void iterate_entire_iso(t_data *img, char*** tab)
{
	int i;
	int j;
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
				p1 = set_point(i,tab[i][j], j);
				p2 = set_point(i,tab[i][j+1], j + 1);
				convert_world_to_screen(p1, p2, img->edgel,img);
			}
			if (i + 1 < img->row)
			{
				p1 = set_point( i,tab[i][j], j);
				p2 = set_point( i + 1, tab[i+1][j], j);
				convert_world_to_screen(p1, p2, img->edgel, img);
			}
			j += 1;
		}
		i += 1;
	}
}
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

char	**put_data_in_table(char **tab, char *line, int totalNumData)
{
	char	*copy;
	char	*start_pos;
	int		countData;
	
	copy = ft_strdup(line);
	countData = 0;
	while(*copy)
	{
		while(*copy && *copy == ' ')
			copy++;
		start_pos = copy;
		while(*copy && *copy != ' ')
			copy++;
		*copy = '\0';
		tab[countData] = start_pos;
		countData++;
		if(countData < totalNumData)
			copy++;
	}
	return tab;
}

int	count_nl(char *file_name)
{
	int		counter;
	char	*line;
	int 	fd;

	counter = -1;
	fd = open(file_name, O_RDONLY);
	line = "a";
	while(line)
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
	int 	fd;
	int		i;
	int 	j;

	i = 0;
	img->row = count_nl(file_name);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return NULL;
	tab = (char***)malloc(img->row * sizeof(char **));
	if(!tab)
		return NULL;
	img->row = 0;
	line = "a";
	while(line)
	{
		line = get_next_line(fd);
		if(line)
		{	
			img->row++;
			img->col = count_split(line, ' ');
			tab[img->row-1] = (char**)malloc(img->col * sizeof(char*));
			if(!tab[img->row-1])
				return NULL;
			tab[img->row-1] = put_data_in_table(tab[img->row-1], line, img->col);
			j = 0;
		}
	}
	img->edgel = (img->window_wigth / cos(26.567 * M_PI / 180) / (img->row + img->col - 2)) * 0.8;
	if (img->edgel == 0)
		return NULL;
	close(fd);
	free(line);
	return tab;
}

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return(0);
}

int	close_window(void *param)
{
	(void)param;
	exit(0);
}

int	main(int ac, char **av)
{
	char	***tab;
	t_data	img;

	img.window_wigth = 700;
	img.widow_height = 500;
	if (ac != 2 || !ft_strnstr(av[1], ".fdf", ft_strlen(av[1])))
		return (0);
	tab = NULL;
	tab = parsing_fdf(tab, av[1], &img);
	if (!tab)
		return 0;
	img.mlx_ptr = mlx_init();
	if (!img.mlx_ptr)
		return (0);
	img.win_ptr = mlx_new_window(img.mlx_ptr, img.window_wigth, img.widow_height, "FDF");
	if (!img.win_ptr)
		return (0);
	img.img = mlx_new_image(img.mlx_ptr, img.window_wigth, img.widow_height );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.linel, &img.endian);
	iterate_entire_iso(&img, tab);
	mlx_put_image_to_window(img.mlx_ptr, img.win_ptr, img.img, 0,0);
	mlx_hook(img.win_ptr, 2, 0, &key_press, &img);
	mlx_hook(img.win_ptr, 17, 0, &close_window, &img);
	mlx_loop(img.mlx_ptr);
}