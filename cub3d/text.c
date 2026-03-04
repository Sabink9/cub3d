#include "cub3d.h"

int	close_window(t_mlx *mlx)
{
	mlx_destroy_window(mlx->mlx, mlx->win);
	mlx_destroy_display(mlx->mlx);
	free(mlx->mlx);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_mlx *mlx)
{
	if (keycode == 65307)
		close_window(mlx);
	return (0);
}

int	main(void)
{
	t_mlx	mlx;
	int		x;
	int		y;
	char	*pixel;
	int	h;
	int	w;

	mlx.mlx = mlx_init();
	if (!mlx.mlx)
		return (1);
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "Animation");
	if (!mlx.win)
		return (1);
	mlx.img = mlx_xpm_file_to_image(mlx.mlx, "../sprites/megaManSheet1.xpm",
			&w, &h);
	if (!mlx.img)
		return (1);
	mlx.addr = mlx_get_data_addr(mlx.img, &mlx.bpp, &mlx.line_len, &mlx.endian);
	if (!mlx.addr)
		return (1);
	y = 150;
	while (y < 250)
	{
		x = 150;
		while (x < 250)
		{
			pixel = mlx.addr + (y * mlx.line_len + x * (mlx.bpp / 8));
			*(unsigned int *)pixel = 0x00FF0000;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img, 0, 0);
	mlx_hook(mlx.win, 17, 0, close_window, &mlx);
	mlx_key_hook(mlx.win, key_hook, &mlx);
	mlx_loop(mlx.mlx);
}
