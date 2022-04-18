#include "mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

typedef struct s_data{
	void	*img;
	int		window_wigth;
	int		widow_height;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
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
	int offset = 150;
	xc = data->window_wigth / 2;
	yc = data->widow_height / 2;
	printf ("v1.y %i, v2.y:%i\n", v1.y, v2.y);
	fromxs = get_screen_x(v1.x, v1.z, r) - get_origin_x(0, 0, r) + xc;
	fromys = get_screen_y(v1.x, v1.z, v1.y, r) - get_origin_y(0, 0, 1, r) + yc;
	toxs = get_screen_x(v2.x, v2.z, r) - get_origin_x(0, 0, r) + xc;
	toys = get_screen_y(v2.x, v2.z, v2.y, r) - get_origin_y(0, 0, 1, r) + yc;
	printf ("from : %d, %d to : %d, %d\n", fromxs, fromys, toxs,toys);
	draw_line(fromxs,fromys, toxs,toys, mlx_ptr, win_ptr, data);
}


void iterate_entire_iso(int x, int y, int r, void* mlx_ptr, void* win_ptr, t_data *data)
{
	int i = 0;
	int j = 0;
	t_vector3	v1;
	t_vector3	v2;
	// printf("go");
	while (j <= y)
	{
		i = 0;
		printf ("\tnew x\n");
		while (i <= x)
		{
			if (j + 1 <= y)
			{	
				 printf("(1 : %i, %i) -> (%i, %i)\n", i, j, i, j+1);
				v1.x = i;
				v1.z = j;
				v1.y = 1;
				v2.x = i;
				v2.y = 1;
				v2.z = j + 1;
				if(i ==1 && j ==1 )
				{
					v1.y = 2;
				}
				if(i ==1 && j+1 ==1 )
				{
					v2.y = 2;
				}

				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, data);
			}
			if (i + 1 <= x)
			{
				 printf("(2 : %i, %i) -> (%i, %i)\n", i, j, i + 1, j);
				v1.x = i;
				v1.z = j;
				v1.y = 1;
				v2.x = i + 1;
				v2.y = 1;
				v2.z = j;
				if(i ==1 && j == 1)
				{
					v1.y =2;
				}
				if(i+1 ==1 && j == 1)
				{
					v2.y =2;
				}
				convert_world_to_screen(v1, v2, r, mlx_ptr, win_ptr, data);
			}
			i += 1;
		}
		j += 1;
	}
}
int	main()
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		x1;
	int		y1;
	t_data	img;
	int edgel = 47;

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
	iterate_entire_iso(2, 2, edgel, mlx_ptr, win_ptr, &img);
	
	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);
	mlx_loop(mlx_ptr);
}