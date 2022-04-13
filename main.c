#include "mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

typedef struct s_data{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vector2{
	float x;
	float y;
}	vector2;

vector2 add_vector2(vector2 a, vector2 b)
{
	vector2 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
}
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
			printf("x negative\n");
	}
	else
		xvalue = 1;
	if (dy < 0)
	{
		yvalue = -1;
		dy *= -1;
			printf("y negative\n");
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
void draw_iso(int x1, int y1, int r, void *mlx_ptr, void *win_ptr, t_data img, int end1, int end2)
{
	int h;
	int w;
	h = r * sin(26.6 * M_PI / 180);
	w = r * cos(26.6 * M_PI / 180);
	printf("%i, %i\n", h, w);
	draw_line(x1, y1, x1 + w/2, y1 + h/2, mlx_ptr, win_ptr, &img);
	if (end1 == 1)
		draw_line(x1 + w/2, y1 + h/2, x1, y1 + h, mlx_ptr, win_ptr, &img);
	if (end1 == 1)
		draw_line(x1, y1 + h, x1 - w/2, y1 + h/2, mlx_ptr, win_ptr, &img);
	draw_line(x1 - w/2, y1 + h/2, x1, y1, mlx_ptr, win_ptr, &img);
	// //if (end2 == 1)
	// draw_line(x1 + r, y1+(r/2), x1, y1+r, mlx_ptr, win_ptr, &img);
	// //if (end1 == 1)
	// draw_line(x1, y1+r, x1-r, y1+(r/2), mlx_ptr, win_ptr, &img);
	// draw_line(x1-r, y1+(r/2), x1, y1, mlx_ptr, win_ptr, &img);
}

int	main()
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		rt;
	int		window_wigth = 1000;
	int		widow_height = 800;
	t_data	img;
	int edge = 100;
	x1 = window_wigth / 2;
	y1 = widow_height / 2;

	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, window_wigth, widow_height, "hello");
	if (!win_ptr)
		return (0);
	int size = 200;
	img.img = mlx_new_image(mlx_ptr, window_wigth, widow_height );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	draw_iso(x1, y1, size, mlx_ptr, win_ptr,img, 1, 0);

	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);

	printf("test");
	mlx_loop(mlx_ptr);
}