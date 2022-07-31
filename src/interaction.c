/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interaction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:02:54 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/30 14:14:15 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include	"../mlx/mlx.h"

void	mlx_free(t_data *img)
{
	mlx_destroy_image(img->mlx_ptr, img->img);
	mlx_destroy_window(img->mlx_ptr, img->win_ptr);
	mlx_destroy_display(img->mlx_ptr);
	mlx_loop_end(img->mlx_ptr);
}

int	close_window(void *param)
{
	int		i;
	int		j;
	t_data	*img;

	img = (t_data *)param;
	(void) param;
	i = 0;
	j = 0;
	mlx_free(img);
	free(img->mlx_ptr);
	while (img->tab[i])
	{
		j = 0;
		while (img->tab[i][j])
			free(img->tab[i][j++]);
		free(img->tab[i++]);
	}
	free(img->tab);
	i = 0;
	while (img->ptab[i])
		free(img->ptab[i++]);
	free(img->ptab);
	exit(0);
}

int	key_press(int keycode, t_data *img)
{
	if (keycode == KEY_ESC)
		close_window((void *)img);
	else if (keycode == KEY_A)
		img->offsetx -= 10;
	else if (keycode == KEY_D)
		img->offsetx += 10;
	else if (keycode == KEY_W)
		img->offsety -= 10;
	else if (keycode == KEY_S)
		img->offsety += 10;
	else if (keycode == KEY_R)
		img->edgel += 1;
	else if (keycode == KEY_F)
		img->edgel -= 1;
	else if (keycode == KEY_T)
		img->mul += 0.1;
	else if (keycode == KEY_G)
		img->mul -= 0.1;
	else
		return (0);
	clear_image(img);
	draw_iso(img);
	mlx_put_image_to_window(img->mlx_ptr, img->win_ptr, img->img, 0, 0);
	return (0);
}

void	mlx_hook_interaction(t_data *img)
{
	mlx_hook(img->win_ptr, 2, 1L << 0, &key_press, img);
	mlx_hook(img->win_ptr, 17, 1L << 0, &close_window, img);
}

void	mlx_functions(t_data *img)
{
	mlx_put_image_to_window(img->mlx_ptr, img->win_ptr, img->img, 0, 0);
	mlx_hook_interaction(img);
	mlx_loop(img->mlx_ptr);
	mlx_destroy_image(img->mlx_ptr, img->img);
	mlx_destroy_window(img->mlx_ptr, img->win_ptr);
	mlx_loop_end(img->mlx_ptr);
}
