/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:41:33 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	check_chars(t_map *map)
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
			if (c != '0' && c != '1' && c != ' ' && c != 'N'
				&& c != 'S' && c != 'E' && c != 'W')
				return (ft_putstr("Error: invalid character in map\n"), 0);
			col++;
		}
		row++;
	}
	return (1);
}

static int	count_players(t_map *map)
{
	int		count;
	int		row;
	int		col;
	char	c;

	count = 0;
	row = 0;
	while (row < map->height)
	{
		col = 0;
		while (col < map->width)
		{
			c = map->grid[row][col];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				count++;
			col++;
		}
		row++;
	}
	return (count);
}

static int	cell_is_closed(t_map *map, int row, int col)
{
	if (row == 0 || row == map->height - 1)
		return (0);
	if (col == 0 || col == map->width - 1)
		return (0);
	if (map->grid[row - 1][col] == ' ')
		return (0);
	if (map->grid[row + 1][col] == ' ')
		return (0);
	if (map->grid[row][col - 1] == ' ')
		return (0);
	if (map->grid[row][col + 1] == ' ')
		return (0);
	return (1);
}

static int	check_closed(t_map *map)
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
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
				if (!cell_is_closed(map, row, col))
					return (ft_putstr("Error: map is not closed\n"), 0);
			col++;
		}
		row++;
	}
	return (1);
}

int	validate_map(t_mlx *data, char *path)
{
	int	len;

	len = ft_strlen(path);
	if (len < 5 || ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (ft_putstr("Error: file must have .cub extension\n"), 0);
	if (!check_chars(&data->map))
		return (0);
	if (count_players(&data->map) != 1)
		return (ft_putstr("Error: map must have exactly one player\n"), 0);
	if (!check_closed(&data->map))
		return (0);
	return (1);
}
