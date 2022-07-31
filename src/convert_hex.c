/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 12:30:47 by hyujung           #+#    #+#             */
/*   Updated: 2022/07/22 12:33:06 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"
#include "../include/libft.h"

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
	int		i;
	int		sum;
	int		len;
	char	*base;

	base = "0123456789ABCDEF";
	i = 0;
	sum = 0;
	len = ft_strlen(str)-1;
	while (str[i] && 0 <= len)
	{
		sum += ft_recursive_power(16, i) * find_inbase(str[len], base);
		i++;
		len--;
	}
	return (sum);
}
