/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:41:30 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:56:59 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	game_loop(t_mlx *data)
{
	double	new_x;
	double	new_y;
	double	orig_x;
	double	orig_y;
	int		moved;

	orig_x = data->player.player_x;
	orig_y = data->player.player_y;
	new_x = orig_x;
	new_y = orig_y;
	moved = 0;
	handle_movement(data, &new_x, &new_y, &moved);
	handle_strafe(data, &new_x, &new_y, &moved);
	handle_rotation(data, &moved);
	if (moved)
	{
		if (check_collision(data, new_x, orig_y))
			data->player.player_x = new_x;
		if (check_collision(data, orig_x, new_y))
			data->player.player_y = new_y;
		render_3d(data);
	}
	return (0);
}

static int	load_textures(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!data->tex[i].path)
			return (printf("Error: missing texture path %d\n", i), 0);
		data->tex[i].img = mlx_xpm_file_to_image(data->mlx,
				data->tex[i].path, &data->tex[i].w, &data->tex[i].h);
		if (!data->tex[i].img)
			return (printf("Error: texture %s\n", data->tex[i].path), 0);
		data->tex[i].addr = mlx_get_data_addr(data->tex[i].img,
				&data->tex[i].bpp, &data->tex[i].line_len,
				&data->tex[i].endian);
		i++;
	}
	data->hands_texture = mlx_xpm_file_to_image(data->mlx,
			"../sprites/hands.xpm", &data->hands_w, &data->hands_h);
	if (!data->hands_texture)
		return (printf("Error: hands texture\n"), 0);
	data->hands_addr = mlx_get_data_addr(data->hands_texture,
			&data->hands_bpp, &data->hands_line_len, &data->hands_endian);
	return (1);
}

static int	init_mlx(t_mlx *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (0);
	data->win = mlx_new_window(data->mlx, WIN_W, WIN_H, "Cub3D");
	if (!data->win)
		return (0);
	if (!load_textures(data))
		return (0);
	data->img = mlx_new_image(data->mlx, WIN_W, WIN_H);
	if (!data->img)
		return (0);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len,
			&data->endian);
	return (1);
}

int	main(int ac, char **av)
{
	t_mlx	data;

	if (ac != 2)
		return (printf("Usage: ./cub3d <map.cub>\n"), 1);
	if (!parse_map(&data, av))
		return (1);
	init_keys(&data);
	if (!init_mlx(&data))
		return (1);
	render_3d(&data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, game_loop, &data);
	mlx_loop(data.mlx);
	return (0);
}
