/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_data_null(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		data->tex[i].path = NULL;
		data->tex[i].img = NULL;
		i++;
	}
	data->mlx = NULL;
	data->win = NULL;
	data->img = NULL;
	data->hands_texture = NULL;
	data->map.grid = NULL;
	data->map.width = 0;
	data->map.height = 0;
}

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

void	find_player(t_map *map, t_player *player)
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
