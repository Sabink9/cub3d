/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:35:53 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/20 15:36:51 by saciurus         ###   ########.fr       */
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
			return (ft_putstr("Error: missing texture path\n"), 0);
		data->tex[i].img = mlx_xpm_file_to_image(data->mlx, data->tex[i].path,
				&data->tex[i].w, &data->tex[i].h);
		if (!data->tex[i].img)
			return (ft_putstr("Error: texture "), ft_putstr(data->tex[i].path),
				ft_putchar('\n'), 0);
		data->tex[i].addr = mlx_get_data_addr(data->tex[i].img,
				&data->tex[i].bpp, &data->tex[i].line_len,
				&data->tex[i].endian);
		i++;
	}
	data->hands_texture = mlx_xpm_file_to_image(data->mlx,
			"../sprites/hands.xpm", &data->hands_w, &data->hands_h);
	if (!data->hands_texture)
		return (ft_putstr("Error: hands texture\n"), 0);
	data->hands_addr = mlx_get_data_addr(data->hands_texture, &data->hands_bpp,
			&data->hands_line_len, &data->hands_endian);
	return (1);
}

static void	destroy_mlx(t_mlx *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
}

static int	init_mlx(t_mlx *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (0);
	data->win = mlx_new_window(data->mlx, WIN_W, WIN_H, "Cub3D");
	if (!data->win)
		return (mlx_destroy_display(data->mlx), free(data->mlx), 0);
	if (!load_textures(data))
		return (unload_tex(data), destroy_mlx(data), 0);
	data->img = mlx_new_image(data->mlx, WIN_W, WIN_H);
	if (!data->img)
		return (unload_tex(data), destroy_mlx(data), 0);
	data->addr = mlx_get_data_addr(data->img, &data->bpp, &data->line_len,
			&data->endian);
	return (1);
}

int	main(int ac, char **av)
{
	t_mlx	data;

	if (ac != 2)
		return (ft_putstr("Usage: ./cub3d <map.cub>\n"), 1);
	if (!parse_map(&data, av))
		return (1);
	init_keys(&data);
	if (!init_mlx(&data))
		return (free_data(&data), 1);
	render_3d(&data);
	mlx_hook(data.win, 2, 1L << 0, (int (*)(void))(void *)key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, (int (*)(void))(void *)key_release, &data);
	mlx_hook(data.win, 17, 0, (int (*)(void))(void *)close_window, &data);
	mlx_loop_hook(data.mlx, (int (*)(void))(void *)game_loop, &data);
	mlx_loop(data.mlx);
	return (0);
}
