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
	int		edgel;
	int		row;
	int		col;
	int	elevation;
}	t_data;

typedef struct s_dot{
	int	row;
	int	height;
	int	col;
	int	color;
} t_dot;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*offset;

	offset = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)offset = color;
}



void draw_line(int x1, int y1, int x2, int y2, void * mlx_ptr, void * win_ptr, t_data *data, int color)
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
			my_mlx_pixel_put(data, x, y, color);		
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
			my_mlx_pixel_put(data, x, y, color);		
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
float get_origin_x(float x, float z, int r)
{
	return (x - z) * cos(26.567 * M_PI / 180) * r;
}
float get_origin_y(float x, float z, int height, int r)
{
	return (((x + z + 1) * sin(26.567 * M_PI / 180) - height) * r);
}

float get_screen_x(int col, int row, int r)
{
	return (col - row) * cos(26.567 * M_PI / 180) * r;
}
float get_screen_y(int col, int row, int height, int r)
{
	return (((col + row) * sin(26.567 * M_PI / 180)) * r) - height;
}

void convert_world_to_screen(t_dot v1, t_dot v2,int r, void * mlx_ptr, void * win_ptr, t_data *data)
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
	// printf ("h = %i\n",tab[data->row/2][data->col/2]);
	// printf("data->col/2: %f, data->row/2: %f\n",data->col/2.0, data->row/2.0);
	fromxs = get_screen_x(v1.col, v1.row, r) - get_origin_x(data->col/2.0, data->row/2.0, r) + xc;
	fromys = get_screen_y(v1.col, v1.row, v1.height, r) - get_origin_y(data->col/2.0, data->row/2.0, 1, r) + yc;
	toxs = get_screen_x(v2.col, v2.row, r) - get_origin_x(data->col/2.0, data->row/2.0, r) + xc;
	toys = get_screen_y(v2.col, v2.row, v2.height, r) - get_origin_y(data->col/2.0, data->row/2.0, 1, r) + yc;
	// printf ("from : %d, %d to : %d, %d, color : %i\n\n", fromxs, fromys, toxs,toys, v1.color);
	draw_line(fromxs,fromys, toxs,toys, mlx_ptr, win_ptr, data, v2.color);
	// draw_line(fromxs,fromys, toxs,toys, mlx_ptr, win_ptr, data, 16711680);
}

int	find_inbase2(char c, char *str)
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
	int	i;
	int	sum;
	int	len;

	i = 0;
	sum = 0;
	len = ft_strlen(str)-1;
	while (str[i] && 0 <= len)
	{
		// printf("power, 16 of power %i, len: %i, str[len] = %c\n", i+1, len, str[len]);
		sum += ft_recursive_power(16, i) * find_inbase2(str[len], "0123456789ABCDEF");
		i++;
		len--;
	}
	return (sum);
}

t_dot	set_dot(int x, char *str, int z)
{
	t_dot	v1;
	char	*color;
	
	v1.row = x;
	v1.col = z;
	color = ft_strchr(str, ',');
	if (color)
	{
		*color = '\0';
		color+=3;
		v1.color = convert_from_str_hex_to_decimal(color);
		printf("color detected : %i\n", v1.color);
	}
	else
		v1.color = 0x00FFFFFF;
	v1.height = ft_atoi(str);
	// printf("height : %d, Color : %i\n", v1.height, v1.color);
	return v1;
}

void iterate_entire_iso(void* mlx_ptr, void* win_ptr, t_data *img, char*** tab)
{
	int i = 0;
	int j = 0;
	int r = img->edgel;

	t_dot	v1;
	t_dot	v2;

	while (i < img->row)
	{
		j = 0;
		while (j < img->col)
		{
			if (j + 1 < img->col)
			{
				v1 = set_dot(i,tab[i][j], j);
				v2 = set_dot(i,tab[i][j+1], j + 1);
				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, img);
			}
			if (i + 1 < img->row)
			{
				v1 = set_dot( i,tab[i][j], j);
				v2 = set_dot( i + 1, tab[i+1][j], j);
				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, img);
			}
			j += 1;
		}
		i += 1;
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

char	**put_data_in_table(char **tab, char *line, int totalNumData)
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
		tab[countData] = start_pos;
		printf("element is : %s\n",	tab[countData]);
		countData++;
		if(countData < totalNumData)
			copy++;
	}
	return tab;
}

int	count_nl(char *file_name)
{
	int	read_byte;
	int	counter;
	char	*line;
	int fd;

	counter = -1;
	fd = open(file_name, O_RDONLY);
	line = "a";
	while(line)
	{
		counter += 1;
		line = get_next_line(fd);
	}
	close(fd);
	// free(line);
	return (counter);
}
char	***create_table(int row, int col)
{
	char	***tab;
	int			i;

	i = 0;
	tab = (char ***)malloc(row * sizeof(char **));
	if (!tab)
		return NULL;
	while (i <row)
	{
		tab[i] = (char**)malloc(col * sizeof(char*));
		if (!tab[i])
			return NULL;
		i++;
	}
	return tab;
}
char	***parsing_fdf(char *file_name, t_data *img)
{
	char	***tab;
	char	*line;
	int		counterRow;
	int		counterCol;
	int 	fd;
	int		i;
	int 	j;
	char	**tmp;
	i = 0;

	counterRow = count_nl(file_name);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return NULL;
	tab = (char***)malloc(counterRow * sizeof(char **));
	if(!tab)
		return NULL;
	counterRow = 0;
	line = "a";
	while(line)
	{
		line = get_next_line(fd);
		// printf("%s\n", line);
		if(line)
		{	
			counterRow++;
			counterCol = count_split(line, ' ');
			tab[counterRow-1] = (char**)malloc(counterCol * sizeof(char*));
			if(!tab[counterRow-1])
				return NULL;
			tab[counterRow-1] = put_data_in_table(tab[counterRow-1], line, counterCol);
			j = 0;
			while(j < counterCol)
			{
				printf("tab[%i][%i], %s\n",counterRow-1, j,tab[counterRow-1][j]);
				j++;
			}
		}
	}
	img->row = counterRow;
	img->col = counterCol;
	// printf("counterRow : %i, counterCol : %i\n", img->row, img->col);
	img->edgel = img->window_wigth / cos(26.567 * M_PI / 180) / (counterRow + counterCol - 2) ;
	printf("edge length :%i\n", img->edgel);
	// printf("width * cos : %f\n",img->window_wigth / cos(26.567 * M_PI / 180));
	close(fd);
	free(line);
	return tab;
}
void	read_table(char **tab, t_data *img)
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
	char	***tab;
	void	*mlx_ptr;
	void	*win_ptr;
	t_data	img;
	// int edgel = 70;
	int fd;
	int row = 3;
	int col = 3;

	img.window_wigth = 700;
	img.widow_height = 600;

	tab = parsing_fdf("test.fdf", &img);
	if (!tab)
		return 0;
	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, img.window_wigth, img.widow_height, "hello");
	if (!win_ptr)
		return (0);
	img.img = mlx_new_image(mlx_ptr, img.window_wigth, img.widow_height );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	iterate_entire_iso(mlx_ptr, win_ptr, &img, tab);
	
	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);
	mlx_loop(mlx_ptr);
}
// either 1 or 0 . everything is in int.
