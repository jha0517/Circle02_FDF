/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:58:56 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/30 14:12:12 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include	<stdio.h>
# include	<unistd.h>
# include	<stdio.h>
# include	<math.h>
# include	<fcntl.h>
# include	<stdlib.h>
# define KEY_ESC	65307
# define KEY_A	97
# define KEY_D	100
# define KEY_W	119
# define KEY_S	115
# define KEY_R	114
# define KEY_T	116
# define KEY_F	102
# define KEY_G	103
# define WINDOW_HEIGHT 1200
# define WINDOW_WIDTH 1600

typedef struct s_point{
	int	row;
	int	height;
	int	col;
	int	screenx;
	int	screeny;
	int	x;
	int	y;
	int	xvalue;
	int	yvalue;
	int	color;
}	t_point;

typedef struct s_data{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img;
	int		win_w;
	int		win_h;
	char	*addr;
	int		bpp;
	int		linel;
	int		endian;
	float	edgel;
	int		row;
	int		col;
	char	***tab;
	t_point	**ptab;
	int		offsetx;
	int		offsety;
	float	mul;
	float	p;
	int		dx;
	int		dy;
}	t_data;

char	***parsing_fdf(char *file_name, t_data *img);
void	iterate_entire_iso(t_data *img);
float	get_origin_x(float x, float z, float r);
float	get_origin_y(float x, float z, int height, float r);
float	get_screen_x(int col, int row, float r);
float	get_screen_y(int col, int row, int height, float r);
void	draw_line(t_point v1, t_point v2, t_data *data);
int		get_col(int isX, int current, t_point v1, t_point v2);
void	mlx_hook_interaction(t_data *img);
int		convert_from_str_hex_to_decimal(char *str);
int		error_image(t_data *img);
int		error_win(t_data *img);
int		error_malloc(char ***tab);
void	img_init(t_data *img);
int		create_tab(char *file_name, t_data *img);
void	mlx_functions(t_data *img);
void	draw_iso(t_data *img);
void	clear_image(t_data *img);
#endif