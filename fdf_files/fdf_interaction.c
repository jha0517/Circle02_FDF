/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_interaction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:02:54 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/19 07:02:02 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	key_press(int keycode, t_data *img)
{
	printf("%i\n", keycode);
	if (keycode == KEY_ESC)
		exit(0);
	if (keycode != KEY_A)
		return (0);
	if (keycode == KEY_A)
	{
		img->img = mlx_new_image(img->mlx_ptr, img->window_wigth, img->widow_height);
		img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->linel, &img->endian);
		mlx_clear_window(img->mlx_ptr, img->win_ptr);
		img->offsetx += 10;
		draw_iso(img);
		mlx_put_image_to_window(img->mlx_ptr, img->win_ptr, img->img, 0, 0);
	}
	if (keycode == 2)
	{
		img->img = mlx_new_image(img->mlx_ptr, img->window_wigth, img->widow_height);
		img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->linel, &img->endian);
		mlx_clear_window(img->mlx_ptr, img->win_ptr);
		img->offsetx -= 10;
		draw_iso(img);
		mlx_put_image_to_window(img->mlx_ptr, img->win_ptr, img->img, 0, 0);
	}
	return (0);
}

int	close_window(void *param)
{
	(void)param;
	exit(0);
}

void	mlx_hook_interaction(t_data *img)
{
	mlx_hook(img->win_ptr, 2, 0, &key_press, img);
	mlx_hook(img->win_ptr, 17, 0, &close_window, img);
}
