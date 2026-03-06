#include "cub3d.h"

int	close_window(t_mlx *data)
{
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
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

void	draw_walls(t_mlx *data)
{
	int	row;
	int	col;

	row = 0;
	while (row < data->map.height)
	{
		col = 0;
		while (col < data->map.width)
		{
			if (data->map.grid[row][col] == '1')
				draw_square(data, col * 64, row * 64, 64, 0x00245969);
			col++;
		}
		row++;
	}
}

void	draw_grid(t_mlx *data, int x, int y)
{
	int	i;
	int	j;

	i = 0;
	while (i <= y)
	{
		j = 0;
		while (j < 64 * x)
		{
			mlx_pixel_put(data->mlx, data->win, j, i * 64, 0x00FFFFFF);
			j++;
		}
		i++;
	}
	j = 0;
	while (j <= x)
	{
		i = 0;
		while (i < 64 * y)
		{
			mlx_pixel_put(data->mlx, data->win, j * 64, i, 0x00FFFFFF);
			i++;
		}
		j++;
	}
}

void	draw_player(t_mlx *data)
{
	int	offset;

	offset = (64 - 20) / 2;
	draw_square(data, data->player.player_x * 64 + offset, data->player.player_y
			* 64 + offset, 20, 0x00FF0000);
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
void	clear_previous_case(t_mlx *data)
{
	draw_square(data, data->player.prev_x * 64, data->player.prev_y * 64, 64,
			0x00000000); // noir = efface
}

void	render(t_mlx *data)
{
	draw_walls(data);
	// 2. redessiner les murs
	draw_grid(data, data->map.width, data->map.height);
	// 3. redessiner la grille
	draw_player(data);
	// 4. redessiner le player
}

int	key_hook(int keycode, t_mlx *data)
{
	int	new_x;
	int	new_y;

	new_x = data->player.player_x;
	new_y = data->player.player_y;
	if (keycode == 65307)
		close_window(data);
	if (keycode == 65362)
		new_y--;
	else if (keycode == 65364)
		new_y++;
	else if (keycode == 65361)
		new_x--;
	else if (keycode == 65363)
		new_x++;
	// vérifie avant de bouger
	if (data->map.grid[new_y][new_x] != '1')
	{
		data->player.prev_x = data->player.player_x; // sauvegarde
		data->player.prev_y = data->player.player_y;
		data->player.player_x = new_x;
		data->player.player_y = new_y;
	}
	clear_previous_case(data); // efface l'ancienne case
	render(data);
	return (0);
}

int	main(int ac, char **av)
{
	t_mlx data;

	if (ac != 2)
		return (printf("ac != 2"), 1);

	parse_map(&data, av);

	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data.win = mlx_new_window(data.mlx, data.map.width * 64, data.map.height
			* 64, "Cub3d");
	if (!data.win)
		return (1);
	render(&data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_key_hook(data.win, key_hook, &data);
	mlx_loop(data.mlx);
	return (0);
}