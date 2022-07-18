/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_interaction.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 17:02:54 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 10:28:56 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"


int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
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