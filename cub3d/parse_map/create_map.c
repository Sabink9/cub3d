/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:41:33 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:10:42 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	set_player(t_map *map, t_player *player, int row, int col)
{
	char	c;

	c = map->grid[row][col];
	player->player_x = col + 0.5;
	player->player_y = row + 0.5;
	if (c == 'N')
		player->dir_angle = 270.0;
	else if (c == 'S')
		player->dir_angle = 90.0;
	else if (c == 'E')
		player->dir_angle = 0.0;
	else
		player->dir_angle = 180.0;
	map->grid[row][col] = '0';
}

static void	find_player(t_map *map, t_player *player)
{
	int		row;
	int		col;
	char	c;

	row = 0;
	while (row < map->height)
	{
		col = 0;
		while (col < map->width)
		{
			c = map->grid[row][col];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				set_player(map, player, row, col);
				return ;
			}
			col++;
		}
		row++;
	}
}

static void	parse_headers(t_mlx *data, int fd, int *flags)
{
	char	*line;

	line = get_next_line(fd);
	while (line && *flags != 63)
	{
		strip_newline(line);
		parse_header_line(data, line, flags);
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}

static int	init_map(t_mlx *data, char *path)
{
	int	fd;
	int	height_flags;

	data->map.grid = NULL;
	data->map.width = 0;
	data->map.height = 0;
	height_flags = 0;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (printf("Error: cannot open file\n"), 0);
	data->map.height = count_map_lines(fd, &height_flags);
	close(fd);
	if (data->map.height == 0)
		return (printf("Error: no map found\n"), 0);
	return (1);
}

int	parse_map(t_mlx *data, char **av)
{
	int	fd;
	int	flags;

	if (!init_map(data, av[1]))
		return (0);
	flags = 0;
	fd = open(av[1], O_RDONLY);
	parse_headers(data, fd, &flags);
	close(fd);
	if (flags != 63)
		return (printf("Error: missing identifiers in .cub file\n"), 0);
	fd = open(av[1], O_RDONLY);
	fill_grid(&data->map, fd);
	close(fd);
	find_player(&data->map, &data->player);
	return (1);
}
