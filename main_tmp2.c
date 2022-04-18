int	main_old()
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		x1;
	int		y1;
	int		window_wigth = 200;
	int		widow_height = 800;
	t_data	img;
	int edge = 100;
	int edgel = 200;
	int size = 200;
	x1 = window_wigth / 2;
	y1 = widow_height / 2;

	int size_xw = 3;
	int size_zw = 6;
	
	printf("good");
	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return (0);
	win_ptr = mlx_new_window(mlx_ptr, window_wigth, widow_height, "hello");
	if (!win_ptr)
		return (0);
	img.img = mlx_new_image(mlx_ptr, window_wigth, widow_height );
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	// draw_iso(x1, y1, size, mlx_ptr, win_ptr,img, 1, 0);
	iterate_entire_iso(2, 3, edgel, mlx_ptr, win_ptr, &img);
	
	mlx_put_image_to_window(mlx_ptr, win_ptr, img.img, 0,0);
	mlx_loop(mlx_ptr);
}