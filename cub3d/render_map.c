#include "cub3d.h"

int	close_window(t_mlx *data)
{
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

int	key_hook(int keycode, t_mlx *data)
{
	if (keycode == 65307)
		close_window(data);
	return (0);
}

void	draw_square(t_mlx *data, int x, int y, int size, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			mlx_pixel_put(data->mlx, data->win, x + j, y + i, color);
			j++;
		}
		i++;
	}
}
char	**copy_map(t_map *map)
{
	int		i;
	char	**copy;

	i = 0;
	while (map->grid[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (map->grid[i])
	{
		copy[i] = ft_strdup(map->grid[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	*find_player(t_map *map, t_player *player)
{
	char **mapc;
	int	**res;
	int	i;
	int	y;
	int	j;
	int	l;

	i = 0;
	y =0;
	j = 0;
	l = 0;
	mapc = copy_map(map);
	while (mapc[i])
	{
		while (mapc[i][y])
		{
			if (mapc[i][y] == 'N')
			{
				j = i;
				l = y;
				return (res[j][l])
			}
				return ()
			y++;
		}
		i++;
	}
	if (mapc)
}
int	main(int ac, char **av)
{
	t_mlx data;
	t_map map;
	
	if (ac != 2)
		return (printf("ac != 2"), 1);

	map.grid = NULL;
	map.width = 0;
	map.height = 0;
	
	parse_map(&map, av);
	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data.win = mlx_new_window(data.mlx, map.width * 64, map.height * 64, "Cub3d");
	if (!data.win)
		return (1);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_key_hook(data.win, key_hook, &data);
	draw_square(&data, 375, 275, 50, 0x00FF0000);
	mlx_loop(data.mlx);
	return (0);
}