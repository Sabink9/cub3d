/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_wall.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 16:12:35 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 16:13:15 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_ray	out_of_bounds(void)
{
	t_ray	result;

	result.distance = 999.0;
	result.hit_x = 0;
	result.hit_y = 0;
	result.side = NORTH;
	return (result);
}

static t_ray	hit_wall_x(t_mlx *data, t_dda *d)
{
	t_ray	result;
	double	perp_dist;

	perp_dist = d->side_dist[0] - d->delta_dist[0];
	result.hit_y = data->player.player_y + perp_dist * d->rdy;
	result.hit_x = 0;
	if (d->step[0] > 0)
		result.side = WEST;
	else
		result.side = EAST;
	if (perp_dist < 0.1)
		perp_dist = 0.1;
	result.distance = perp_dist;
	return (result);
}

t_ray	hit_wall(t_mlx *data, t_dda *d, int side)
{
	t_ray	result;
	double	perp_dist;

	if (side == 0)
		return (hit_wall_x(data, d));
	perp_dist = d->side_dist[1] - d->delta_dist[1];
	result.hit_x = data->player.player_x + perp_dist * d->rdx;
	result.hit_y = 0;
	if (d->step[1] > 0)
		result.side = NORTH;
	else
		result.side = SOUTH;
	if (perp_dist < 0.1)
		perp_dist = 0.1;
	result.distance = perp_dist;
	return (result);
}
