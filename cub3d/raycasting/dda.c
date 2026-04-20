/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 16:10:53 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/20 15:26:38 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	init_dda_x(t_mlx *data, t_dda *d)
{
	if (d->rdx != 0)
		d->delta_dist[0] = fabs(1.0 / d->rdx);
	else
		d->delta_dist[0] = 1e30;
	if (d->rdx < 0)
	{
		d->step[0] = -1;
		d->side_dist[0] = (data->player.player_x - d->map[0])
			* d->delta_dist[0];
	}
	else
	{
		d->step[0] = 1;
		d->side_dist[0] = (d->map[0] + 1.0 - data->player.player_x)
			* d->delta_dist[0];
	}
}

static void	init_dda_y(t_mlx *data, t_dda *d)
{
	if (d->rdy != 0)
		d->delta_dist[1] = fabs(1.0 / d->rdy);
	else
		d->delta_dist[1] = 1e30;
	if (d->rdy < 0)
	{
		d->step[1] = -1;
		d->side_dist[1] = (data->player.player_y - d->map[1])
			* d->delta_dist[1];
	}
	else
	{
		d->step[1] = 1;
		d->side_dist[1] = (d->map[1] + 1.0 - data->player.player_y)
			* d->delta_dist[1];
	}
}

void	init_dda(t_mlx *data, double angle, t_dda *d)
{
	d->rdx = cos(angle * M_PI / 180.0);
	d->rdy = sin(angle * M_PI / 180.0);
	d->map[0] = (int)data->player.player_x;
	d->map[1] = (int)data->player.player_y;
	init_dda_x(data, d);
	init_dda_y(data, d);
}

void	dda_step(t_dda *d, int *side)
{
	if (d->side_dist[0] < d->side_dist[1])
	{
		d->side_dist[0] += d->delta_dist[0];
		d->map[0] += d->step[0];
		*side = 0;
	}
	else
	{
		d->side_dist[1] += d->delta_dist[1];
		d->map[1] += d->step[1];
		*side = 1;
	}
}
