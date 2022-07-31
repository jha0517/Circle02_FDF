/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:52:47 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/27 13:54:33 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"include/fdf.h"
#include	"mlx/mlx.h"
#include	"include/libft.h"

int	main(int ac, char **av)
{
	t_data	img;

	img_init(&img);
	if (ac != 2 || !ft_strnstr(av[1], ".fdf", ft_strlen(av[1])))
		return (0);
	img.mlx_ptr = mlx_init();
	if (!img.mlx_ptr)
		return (0);
	img.tab = parsing_fdf(av[1], &img);
	if (!img.tab)
		return (error_malloc(img.tab));
	img.win_ptr = mlx_new_window(img.mlx_ptr, img.win_w, img.win_h, "FDF");
	if (!img.win_ptr)
		return (error_win(&img));
	img.img = mlx_new_image(img.mlx_ptr, img.win_w, img.win_h);
	if (!img.img)
		return (error_image(&img));
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.linel, &img.endian);
	iterate_entire_iso(&img);
	mlx_functions(&img);
}
