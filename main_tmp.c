#include "mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

typedef struct s_data{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*offset;

	offset = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	printf ("line_length %i\n", data->line_length);
	printf ("bits_per_pixel %i\n", data->bits_per_pixel);

	*(unsigned int*)offset = color;
}

int	main()
{
	void	*mlx;
	void 	*mlx_win;
	t_data	img;


	printf ("main : line_length %i\n", img.line_length);
	printf ("main : bits_per_pixel %i\n", img.bits_per_pixel);

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 500, 200, "My window");
	img.img = mlx_new_image(mlx, 500, 200 );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

	my_mlx_pixel_put(&img, 50,100, 0x00FFFFFF);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0,0);
	mlx_loop(mlx);
}