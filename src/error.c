/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 11:39:18 by hyujung           #+#    #+#             */
/*   Updated: 2022/07/26 11:05:11 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../mlx/mlx.h"
#include "../include/fdf.h"

int	error_image(t_data *img)
{
	free(img->tab);
	mlx_destroy_image(img->mlx_ptr, img->img);
	return (0);
}

int	error_win(t_data *img)
{
	free(img->tab);
	mlx_destroy_window(img->mlx_ptr, img->win_ptr);
	return (0);
}

int	error_malloc(char ***tab)
{
	free(tab);
	return (0);
}

void	img_init(t_data *img)
{
	img->win_w = WINDOW_WIDTH;
	img->win_h = WINDOW_HEIGHT;
	img->ptab = NULL;
	img->tab = NULL;
	img->offsetx = 0;
	img->offsety = 0;
	img->mul = 1;
}
