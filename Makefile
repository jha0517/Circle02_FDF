
NAME	= fdf

SRCS	= get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
libft/ft_atoi.c libft/ft_recursive_power.c libft/ft_split.c libft/ft_strchr.c \
libft/ft_strdup.c libft/ft_strlen.c main.c libft/ft_strnstr.c \
fdf_files/fdf_color.c fdf_files/fdf_draw_iso.c fdf_files/fdf_draw_line.c \
fdf_files/fdf_interaction.c fdf_files/fdf_math.c fdf_files/fdf_parsing.c

OBJS	= ${SRCS:.c=.o}

CC		= cc

CFLAGS	= -Wall -Werror -Wextra 

${NAME}: 	${OBJS}
			${CC} -o ${NAME} ${OBJS} mlx/libmlx.a ${CFLAGS} -I. -Lmlx -lmlx -framework OpenGL -framework Appkit -fsanitize=address

all: 		${NAME}

clean:
			rm -f ${OBJS}

fclean: 	clean
			rm -f ${NAME}

re: 		fclean all

.PHONY: all clean fclean re bonus


