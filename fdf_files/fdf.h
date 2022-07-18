/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 16:58:56 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 13:08:30 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	<stdio.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<math.h>
#include	<fcntl.h>
#include	"../get_next_line/get_next_line.h"
#include	"../libft/libft.h"
#include	"../mlx/mlx.h"
#define	KEY_ESC	53

typedef struct s_data{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img;
	int		window_wigth;
	int		widow_height;
	char	*addr;
	int		bpp;
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
	int	x;
	int	y;
	int	xvalue;
	int	yvalue;
	int	color;
}	t_point;

char	***parsing_fdf(char ***tab, char *file_name, t_data *img);
void	iterate_entire_iso(t_data *img, char ***tab);
float	get_origin_x(float x, float z, float r);
float	get_origin_y(float x, float z, int height, float r);
float	get_screen_x(int col, int row, float r);
float	get_screen_y(int col, int row, int height, float r);
void	draw_line(t_point v1, t_point v2, t_data *data);
int		get_col(int isX, int current, t_point v1, t_point v2);
void	mlx_hook_interaction(t_data *img);
int		convert_from_str_hex_to_decimal(char *str);
