/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_and_window.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:31:27 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:31:42 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	unload_tex(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->tex[i].img)
			mlx_destroy_image(data->mlx, data->tex[i].img);
		i++;
	}
	if (data->hands_texture)
		mlx_destroy_image(data->mlx, data->hands_texture);
}

int	close_window(t_mlx *data)
{
	free_data(data);
	unload_tex(data);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

int	key_press(int keycode, t_mlx *data)
{
	if (keycode == 65307)
		close_window(data);
	if (keycode < 65536)
		data->keys[keycode] = 1;
	return (0);
}

int	key_release(int keycode, t_mlx *data)
{
	if (keycode < 65536)
		data->keys[keycode] = 0;
	return (0);
}

void	init_keys(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 65536)
	{
		data->keys[i] = 0;
		i++;
	}
}
