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
	float	edgel;
	int		row;
	int		col;
	int	elevation;
}	t_data;

typedef struct s_color{
	int trgb;
	int	r;
	int	g;
	int	b;
} t_color;

typedef struct s_dot{
	int	row;
	int	height;
	int	col;
	t_color color;
} t_dot;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*offset;

	offset = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)offset = color;
}

void	debug_print(char c, int x, int y, int color)
{
	printf("%c axis, (%i, %i), color: %i\n", c, x, y, color);

}

int	create_trgb(int t, int r, int g, int b)
{
	return (t<<24 | r << 16 | g << 8 | b);
}

t_color	set_color(int r, int g, int b)
{
	t_color color;

	color.trgb = create_trgb(0, r, g, b);
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

int	get_r(int trgb)
{
	return ((trgb >> 16) & 0xFF);
}

int	get_g(int trgb)
{
	return ((trgb >> 8) & 0xFF);
}

int	get_b(int trgb)
{
	return (trgb & 0xFF);
}

double	get_ratio(int current, int start, int end)
{
	double ratio;
	if (start == end)
		return (1.0);
	// printf("current: %i,start :%i,end : %i\n", current, start,end);
	ratio = (double)(current-start) / (end - start);
	return ratio;
}

static int	process_lerp(int s, int f, double ratio)
{
	return ((int)((ratio) * s + (1 - ratio) * f));
}

int	ab(int a)
{
	int ab;

	if (a < 0)
		return a * -1;
	return ab;
}

int	get_current_color(int current, int start, int end, t_color *fromcol, t_color *tocol)
{
	double	ratio;
	int		red;
	int		green;
	int		blue;

	if (fromcol->trgb == tocol->trgb)
		return (fromcol->trgb);
	ratio = get_ratio(current, start, end);
	// printf("%f\n", ratio);
	red = process_lerp((tocol->r) & 0xFF, (fromcol->r) & 0xFF, ratio);
	green = process_lerp((tocol->g) & 0xFF, (fromcol->g) & 0xFF, ratio);
	blue = process_lerp((tocol->b) & 0xFF, (fromcol->b) & 0xFF, ratio);

	return ((red << 16) | (green << 8) | blue);
}

void draw_line(int x1, int y1, int x2, int y2, void * mlx_ptr, void * win_ptr, t_data *data, t_color *fromcolor, t_color *tocolor)
{
	int	x = x1;
	int	y = y1;
	int dx;
	int	dy;
	float p;
	int xvalue;
	int yvalue;
	int	color;

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
			color = get_current_color(x, x1, x2, fromcolor, tocolor);
			// debug_print('x', x, y, color);
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
			color = get_current_color(y, y1, y2, fromcolor, tocolor);
			// debug_print('y', x, y, color);
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
float get_origin_x(float x, float z, float r)
{
	return (x - z) * cos(26.567 * M_PI / 180) * r;
}
float get_origin_y(float x, float z, int height, float r)
{
	return (((x + z + 1) * sin(26.567 * M_PI / 180) - height) * r);
}

float get_screen_x(int col, int row, float r)
{
	return (col - row) * cos(26.567 * M_PI / 180) * r;
}
float get_screen_y(int col, int row, int height, float r)
{
	return (((col + row) * sin(26.567 * M_PI / 180)) * r) - height;
}

void convert_world_to_screen(t_dot v1, t_dot v2,float r, void * mlx_ptr, void * win_ptr, t_data *data)
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
	draw_line(fromxs,fromys, toxs,toys, mlx_ptr, win_ptr, data, &v1.color, &v2.color);
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
	char	*color_str;
	char	*start;

	v1.row = x;
	v1.col = z;
	start = ft_strdup(str);
	color_str = ft_strchr(start, ',');
	if (color_str)
	{
		*color_str = '\0';
		color_str+=3;
		v1.color.trgb = convert_from_str_hex_to_decimal(color_str);
		v1.color.r = get_r(v1.color.trgb);
		v1.color.g = get_g(v1.color.trgb);
		v1.color.b = get_b(v1.color.trgb);
	}
	else
	{
		v1.color.trgb = 0x00FFFFFF;
		v1.color.r = 255;
		v1.color.g = 255;
		v1.color.b = 255;
	}
	v1.height = ft_atoi(start);
	// printf("height : %d, Color : %i\n", v1.height, v1.color);
	return v1;
}

void iterate_entire_iso(void* mlx_ptr, void* win_ptr, t_data *img, char*** tab)
{
	int i = 0;
	int j = 0;
	float r = img->edgel;

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
				// printf("(%i, %i) col %i -> (%i, %i) col %i\n", i, j, v1.color.b, i, j+1, v2.color.b);
				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, img);
			}
			if (i + 1 < img->row)
			{
				v1 = set_dot( i,tab[i][j], j);
				v2 = set_dot( i + 1, tab[i+1][j], j);
				// printf("(%i, %i) col %i -> (%i, %i) col %i\n", i, j, v1.color.b, i+1, j, v2.color.b);
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
		// printf("element is : %s\n",	tab[countData]);
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
			// while(j < counterCol)
			// {
			// 	printf("tab[%i][%i], %s\n",counterRow-1, j,tab[counterRow-1][j]);
			// 	j++;
			// }
		}
	}
	img->row = counterRow;
	img->col = counterCol;
	printf("counterRow : %i, counterCol : %i, calcul : %f\n", img->row, img->col, cos(26.567 * M_PI / 180));
	// printf()
	img->edgel = img->window_wigth / cos(26.567 * M_PI / 180) / (counterRow + counterCol - 2) ;
	printf("edge length :%f\n", img->edgel);
	if (img->edgel == 0)
		return NULL;
	// printf("width * cos : %f\n",img->window_wigth / cos(26.567 * M_PI / 180));
	close(fd);
	free(line);
	return tab;
}
// void	read_table(char **tab, t_data *img)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while(i < img->row)
// 	{
// 		j = 0;
// 		while(j < img->col)
// 		{
// 			printf (" (%i, %i)\n",i, j);
// 			printf("tab[%i][%i] : %i\n", i, j, tab[i][j]);
// 			j++;
// 		}
// 		i++;
// 	}
// }

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
	t_color fromcol;
	t_color tocol;

	img.window_wigth = 1500;
	img.widow_height = 1000;

	tab = parsing_fdf("test_maps/elem-fract.fdf", &img);
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
	
	// fromcol = set_color(255, 0, 0);
	// tocol = set_color(255, 255, 255);
	// printf("tocol %i\n",fromcol.trgb);
	// printf("fromcol %i\n",fromcol.trgb);
	// draw_line(0,0, 50,200, mlx_ptr, win_ptr, &img, &fromcol, &tocol);

	iterate_entire_iso(mlx_ptr, win_ptr, &img, tab);
	
	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);
	mlx_loop(mlx_ptr);
}
// either 1 or 0 . everything is in int.
