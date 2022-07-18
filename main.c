/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:52:47 by hyunahjung        #+#    #+#             */
/*   Updated: 2022/07/18 10:30:41 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_files/fdf.h"

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
		return (0);
	img.mlx_ptr = mlx_init();
	if (!img.mlx_ptr)
		return (0);
	img.win_ptr = mlx_new_window(img.mlx_ptr, img.window_wigth, img.widow_height, "FDF");
	if (!img.win_ptr)
		return (0);
	img.img = mlx_new_image(img.mlx_ptr, img.window_wigth, img.widow_height);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.linel, &img.endian);
	iterate_entire_iso(&img, tab);
	mlx_put_image_to_window(img.mlx_ptr, img.win_ptr, img.img, 0, 0);
	mlx_hook_interaction(&img);
	mlx_loop(img.mlx_ptr);
}
