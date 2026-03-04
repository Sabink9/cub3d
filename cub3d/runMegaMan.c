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

	// Initialiser MLX
	mlx.mlx = mlx_init();
	if (!mlx.mlx)
		return (1);
	mlx.win = mlx_new_window(mlx.mlx, 800, 600, "Animation");
	if (!mlx.win)
		return (1);
}
