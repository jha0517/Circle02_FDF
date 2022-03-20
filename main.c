#include "mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

void ft_putchar(char c)
{
	write(1, &c, 1);
}
int key_press(int key, void *param)
{
	printf("key press\n");
	return (0);
}
int key_release(int key, void *param)
{
	printf("key release\n");
	return (0);
}

int mouse_press(int key, void *param)
{
	printf("mouse press\n");
	return (0);
}
int mouse_release(int key, void *param)
{
	printf("mouse release\n");
	return (0);
}
int mouse_move(int key, void *param)
{
	//printf("move\n");
	return (0);
}
int expose(int key, void *param)
{
	printf("expose\n");
	return (0);
}

void draw_line(int x1, int y1, int x2, int y2, void * mlx_ptr, void * win_ptr)
{
	int	x = x1;
	int	y = y1;
	int dx;
	int	dy;
	int p;
	int m;

	dx = x2-x1;
	dy = y2-y1;
	p = 2 * dy - dx;
	m = dy / dx;
	printf("dx : %i, dy : %i, p : %i, m : %i\n", dx, dy, p, m);
	// if (dx < 0)
	// {
	// 	m *= -1;

	// }
	// //if ()
	if (m == 0)
	{
		while (x <= x2)
		{
			printf("if1, x : %i, y : %i, p : %i\n", x, y, p);
			mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x00FFFFFF);		
			x++;
			if (p < 0)
				p = p + (2 * dy);
			else
			{
				p = p + (2 * dy) - (2 * dx);
				y++;
			}
		}
	}
	else if (m > 0)
	{
		while (y <= y2)
		{
			printf("if2, x : %i, y : %i, p : %i\n", x, y, p);
			mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x00FFFFFF);		
			y++;
			if (p < 0)
				p = p + (2 * dx);
			else
			{
				p = p + (2 * dx) - (2 * dy);
				x++;
			}
		}
	}
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
	int		iso_height;
	int		iso_width = 50;
	int		window_wigth = 500;
	int		widow_height = 400;

	iso_height = iso_width / 2;
	x1 = window_wigth / 2;
	y1 = 0;

	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, window_wigth, widow_height, "hello");
	if (!win_ptr)
		return (0);
	x2 = x1 - 100;
	y2 = y1 + 50;
	printf ("x2 : %i, y2 : %i\n", x2, y2);
	draw_line(x1, y1, x2, y2, mlx_ptr, win_ptr);
	// x2 = x1 - 2 * size;
	// y2 = y1 + size;
	// draw_line(x1, y1, x2, y2, mlx_ptr, win_ptr);
	

	//mlx_hook(win_ptr, 2, 0, key_press, (void *)0);
	//mlx_hook(win_ptr, 3, 0, key_release, (void *)0);
	//mlx_hook(win_ptr, 4, 0, mouse_press, (void *)0);
	//mlx_hook(win_ptr, 5, 0, mouse_release, (void *)0);
	//mlx_hook(win_ptr, 6, 0, mouse_move, (void *)0);
	//mlx_hook(win_ptr, 12, 0, expose, (void *)0);

	mlx_loop(mlx_ptr);
	//mlx_destroy_window(mlx_ptr, win_ptr);
}
