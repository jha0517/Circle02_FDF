/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunahjung <hyunahjung@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 20:25:14 by hyujung           #+#    #+#             */
/*   Updated: 2022/07/18 10:41:23 by hyunahjung       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

char	*getl_m_beforenl_ret_beforenl_w_strnull(char *backup);
char	*join_m_bcplusbuf_ft_bc_ret_bcplusbuf(char *backup, char *buf);
char	*ft_find_c_in_str(char *backup, int nl);
char	*readl_m_buf_fr_buf_ret_bc_w_nlnullstr(int fd, char *backup);
char	*newbc_m_afternl_strnull_fr_bc_ret_afternl(char *backup);
char	*get_next_line(int fd);
int		sln(char *str);

#endif
