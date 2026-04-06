/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moovs_and_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:34:07 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:55:06 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	handle_movement(t_mlx *data, double *new_x, double *new_y, int *moved)
{
	double	move_speed;
	double	dir_x;
	double	dir_y;

	move_speed = 0.05 * WIN_W / 800.0;
	dir_x = cos(data->player.dir_angle * M_PI / 180.0);
	dir_y = sin(data->player.dir_angle * M_PI / 180.0);
	if (data->keys[119])
	{
		*new_x += dir_x * move_speed;
		*new_y += dir_y * move_speed;
		*moved = 1;
	}
	if (data->keys[115])
	{
		*new_x -= dir_x * move_speed;
		*new_y -= dir_y * move_speed;
		*moved = 1;
	}
}

void	handle_strafe(t_mlx *data, double *new_x, double *new_y, int *moved)
{
	double	move_speed;
	double	dir_x;
	double	dir_y;

	move_speed = 0.05 * WIN_W / 800.0;
	dir_x = cos(data->player.dir_angle * M_PI / 180.0);
	dir_y = sin(data->player.dir_angle * M_PI / 180.0);
	if (data->keys[97])
	{
		*new_x += dir_y * move_speed;
		*new_y -= dir_x * move_speed;
		*moved = 1;
	}
	if (data->keys[100])
	{
		*new_x -= dir_y * move_speed;
		*new_y += dir_x * move_speed;
		*moved = 1;
	}
}

void	handle_rotation(t_mlx *data, int *moved)
{
	double	rot_speed;

	rot_speed = 1.5 * WIN_W / 800.0;
	if (data->keys[65361])
	{
		data->player.dir_angle -= rot_speed;
		if (data->player.dir_angle < 0)
			data->player.dir_angle += 360.0;
		*moved = 1;
	}
	if (data->keys[65363])
	{
		data->player.dir_angle += rot_speed;
		if (data->player.dir_angle >= 360.0)
			data->player.dir_angle -= 360.0;
		*moved = 1;
	}
}

int	check_row(t_mlx *data, int y, int x_min, int x_max)
{
	int	x;

	if (y < 0 || y >= data->map.height)
		return (0);
	x = x_min;
	while (x <= x_max)
	{
		if (x < 0 || x >= data->map.width || data->map.grid[y][x] == '1')
			return (0);
		x++;
	}
	return (1);
}

int	check_collision(t_mlx *data, double new_x, double new_y)
{
	t_collision	c;
	int			y;

	c.margin = 0.35;
	c.x_min = (int)(new_x - c.margin);
	c.x_max = (int)(new_x + c.margin);
	c.y_min = (int)(new_y - c.margin);
	c.y_max = (int)(new_y + c.margin);
	y = c.y_min;
	while (y <= c.y_max)
	{
		if (!check_row(data, y, c.x_min, c.x_max))
			return (0);
		y++;
	}
	return (1);
}
