#include "mlx/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include "get_next_line/get_next_line.h"
#include "libft/libft.h"

typedef struct s_data{
	void	*img;
	int		window_wigth;
	int		widow_height;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		row;
	int		col;
}	t_data;

typedef struct s_vector3{
	int	x;
	int	y;
	int	z;
} t_vector3;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*offset;

	offset = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)offset = color;
}

void draw_line(int x1, int y1, int x2, int y2, void * mlx_ptr, void * win_ptr, t_data *data)
{
	int	x = x1;
	int	y = y1;
	int dx;
	int	dy;
	float p;
	int xvalue;
	int yvalue;

	dx = x2-x1;
	dy = y2-y1;

	if (dx < 0)
	{
			xvalue = -1;
			dx *= -1;
			// printf("x negative\n");
	}
	else
		xvalue = 1;
	if (dy < 0)
	{
		yvalue = -1;
		dy *= -1;
			// printf("y negative\n");
	}
	else
		yvalue = 1;
	p = 2 * dy - dx;
	if (dx > dy)
	{
		while (x != x2)
		{
			my_mlx_pixel_put(data, x, y, 0x00FFFFFF);		
			x+= xvalue;
			if (p < 0)
				p = p + (2 * dy);
			else
			{
				p = p + (2 * dy) - (2 * dx);
				y+= yvalue;
			}
		}
	}
	else
	{
		while (y != y2)
		{
			my_mlx_pixel_put(data, x, y, 0x00FFFFFF);		
			y+= yvalue;
			if (p < 0)
				p = p + (2 * dx);
			else
			{
				p = p + (2 * dx) - (2 * dy);
				x+= xvalue;
			}
		}
	}
}
float get_origin_x(int x, int z, int r)
{
	return (x - z) * cos(26.565 * M_PI / 180) * r;
}
float get_origin_y(int x, int z, int height, int r)
{
	return ((x + z + 1) * sin(26.565 * M_PI / 180) - height) * r;
}

float get_screen_x(int x, int z, int r)
{
	return (x - z) * cos(26.565 * M_PI / 180) * r;
}
float get_screen_y(int x, int z, int height, int r)
{
	return ((x + z) * sin(26.565 * M_PI / 180) - height) * r;
}

void convert_world_to_screen(t_vector3 v1, t_vector3 v2,int r, void * mlx_ptr, void * win_ptr, t_data *data)
{
	int fromxs;
	int fromys;
	int toxs;
	int toys;
	int xc;
	int yc;

	xc = data->window_wigth / 2;
	yc = data->widow_height / 2;
	// printf ("v1.y %i, v2.y:%i\n", v1.y, v2.y);
	fromxs = get_screen_x(v1.x, v1.z, r) - get_origin_x(0, 0, r) + xc;
	fromys = get_screen_y(v1.x, v1.z, v1.y, r) - get_origin_y(0, 0, 1, r) + yc;
	toxs = get_screen_x(v2.x, v2.z, r) - get_origin_x(0, 0, r) + xc;
	toys = get_screen_y(v2.x, v2.z, v2.y, r) - get_origin_y(0, 0, 1, r) + yc;
	// printf ("from : %d, %d to : %d, %d\n", fromxs, fromys, toxs,toys);
	draw_line(fromxs,fromys, toxs,toys, mlx_ptr, win_ptr, data);
}

t_vector3	set_vector3(int x, int y, int z)
{
	t_vector3	v1;
	
	v1.x = x;
	v1.y = y;
	v1.z = z;

	return v1;
}

void iterate_entire_iso(int r, void* mlx_ptr, void* win_ptr, t_data *img, int** tab)
{
	int i = 0;
	int j = 0;
	// int x = data->row;
	// int y = data->col;
	// printf("x : %i\n", data->row);
	// printf("y : %i\n", data->col);

	t_vector3	v1;
	t_vector3	v2;

	while (j < img->col)
	{
		i = 0;
		while (i < img->row)
		{
			if (j + 1 < img->col)
			{
				printf("(%i, %i)-> (%i, %i)\n", i, j, i, j+1);
				printf("(height %i)-> (height%i)\n", tab[i][j], tab[i][j+1]);

				v1 = set_vector3(i, tab[i][j], j);
				v2 = set_vector3(i, tab[i][j+1], j + 1);
				// printf("\t1***i : %i, j: %i\n", i, j);
				// printf("1) from v1 : %i, %i, height : %i\n", v1.x, v1.z, v1.y);
				// printf("1) to v2 : %i, %i, height : %i\n", v2.x, v2.z, v2.y);

				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, img);
			}
			if (i + 1 < img->row)
			{
				printf("(%i, %i)-> (%i, %i)\n", i, j, i+1, j);
				printf("(height %i)-> (height%i)\n", tab[i][j], tab[i+1][j]);
				// printf("Helo\n");
				v1 = set_vector3(i, tab[i][j], j);
				v2 = set_vector3(i + 1, tab[i+1][j], j);
				// printf("tab[0][2] : %i\n", tab[0][2]);
				// printf("\t2***i : %i, j: %i\n", i, j);
				// printf("2) from v1 : %i, %i, height : %i\n", v1.x, v1.z, v1.y);
				// printf("2) to v2 : %i, %i, height : %i\n", v2.x, v2.z, v2.y);

				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, img);
			}
			i += 1;
		}
		j += 1;
	}
}
static int	count_split(const char *str, char charset)
{
	int	counter;
	int	i;

	i = 0;
	counter = 0;
	while (str[i] && str[i] == charset)
		i++;
	while (str[i])
	{
		counter++;
		while (str[i] != charset && str[i])
			i++;
		while (str[i] == charset && str[i])
			i++;
	}
	return (counter);
}

int	*put_data_in_table(int *tab, char *line, int totalNumData)
{
	char	*copy;
	char	*start_pos;
	int		countData;
	
	copy = ft_strdup(line);
	countData = 0;
	while(*copy)
	{
		while(*copy && *copy == ' ')
			copy++;
		start_pos = copy;
		while(*copy && *copy != ' ')
			copy++;
		*copy = '\0';
		tab[countData] = ft_atoi(start_pos);
		printf("element is : %i\n",tab[countData]);
		countData++;
		if(countData < totalNumData)
			copy++;
	}
	return tab;
}

int	**parsing_fdf(char *file_name, t_data *img)
{
	int		**tab;
	char	*line;
	int		counterRow;
	int		counterCol;
	int 	fd;
	int		i;
	int 	j;

	i = 0;
	counterRow = 0;
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return NULL;
	tab = (int**)malloc(1 * sizeof(int *));
	if(!tab)
		return NULL;
	line = "a";
	while(line)
	{
		line = get_next_line(fd);
		if(line)
		{
			counterRow += 1;
			tab = (int**)realloc(tab, counterRow * sizeof(int*));
			if(!tab)
				return NULL;
			counterCol = count_split(line, ' ');
			// printf("counterCol : %i\n", counterCol);
			tab[counterRow-1] = (int*)malloc(counterCol * sizeof(int));
			if(!tab[counterRow-1])
				return NULL;
			tab[counterRow-1] = put_data_in_table(tab[counterRow-1], line, counterCol);
			j = 0;
			while(j < counterCol)
			{
				// printf("tab[%i][%i], %i\n",counterRow-1, j,tab[counterRow-1][j]);
				j++;
			}
		}
	}
	img->row = counterRow;
	img->col = counterCol;
	close(fd);
	free(line);
	return tab;
}
void	read_table(int **tab, t_data *img)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while(i < img->row)
	{
		j = 0;
		while(j < img->col)
		{
			printf (" (%i, %i)\n",i, j);
			printf("tab[%i][%i] : %i\n", i, j, tab[i][j]);
			j++;
		}
		i++;
	}
}

int	main(void)
{
	int	**tab;
	void	*mlx_ptr;
	void	*win_ptr;
	t_data	img;
	int edgel = 70;
	int fd;
	int row = 3;
	int col = 3;
	// tab = create_table(row, col);

	tab = parsing_fdf("test.fdf", &img);
	// printf("col : %i\n",img.col);
	// printf("row : %i\n",img.row);
	// read_table(tab, &img);
	img.window_wigth = 500;
	img.widow_height = 500;

	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, img.window_wigth, img.widow_height, "hello");
	if (!win_ptr)
		return (0);
	img.img = mlx_new_image(mlx_ptr, img.window_wigth, img.widow_height );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	iterate_entire_iso(edgel, mlx_ptr, win_ptr, &img, tab);
	
	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);
	mlx_loop(mlx_ptr);
}