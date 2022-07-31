
NAME	= fdf

SRCS	= get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
libft/ft_atoi.c libft/ft_recursive_power.c libft/ft_split.c libft/ft_strchr.c \
libft/ft_strdup.c libft/ft_strlen.c main.c libft/ft_strnstr.c libft/ft_bzero.c \
src/color.c src/draw_iso.c src/draw_line.c src/error.c src/convert_hex.c \
src/interaction.c src/math.c src/parsing.c src/parsing2.c

OBJS	= ${SRCS:.c=.o}

CC		= cc

CFLAGS	= -Wall -Werror -Wextra 

${NAME}: 	${OBJS}
			${CC} -o ${NAME} ${OBJS} mlx/libmlx.a ${CFLAGS} -I. -Lmlx -lmlx -lXext -lX11 -lm
all: 		${NAME}

clean:
			rm -f ${OBJS}

fclean: 	clean
			rm -f ${NAME}

re: 		fclean all

norminette:
			norminette	src
			norminette	libft
			norminette	get_next_line
			norminette	include

.PHONY: all clean fclean re
