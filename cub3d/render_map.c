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
	int		i;
	int		j;
	int		pixel_x;
	int		pixel_y;
	char	*pixel;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			pixel_x = x + j;
			pixel_y = y + i;
			if (pixel_x >= 0 && pixel_x < 800 && pixel_y >= 0 && pixel_y < 600)
			{
				pixel = data->addr + (pixel_y * data->line_len + pixel_x
						* (data->bpp / 8));
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
}

void	draw_hands(t_mlx *data)
{
	int				hands_start_y;
	int				hands_start_x;
	int				hy;
	int				hx;
	int				screen_x;
	int				screen_y;
	char			*pixel_src;
	unsigned int	color;
	int				r;
	int				g;
	int				b;
	char			*pixel_dst;

	hands_start_y = 600 - data->hands_h;
	hands_start_x = (800 - data->hands_w) / 2;
	hy = 0;
	while (hy < data->hands_h)
	{
		hx = 0;
		while (hx < data->hands_w)
		{
			screen_x = hands_start_x + hx;
			screen_y = hands_start_y + hy;
			if (screen_x >= 0 && screen_x < 800 && screen_y >= 0
				&& screen_y < 600)
			{
				pixel_src = data->hands_addr + (hy * data->hands_line_len + hx
						* (data->hands_bpp / 8));
				color = *(unsigned int *)pixel_src;
				r = (color >> 16) & 0xFF;
				g = (color >> 8) & 0xFF;
				b = color & 0xFF;
				if (!(g > 80 && g > r + 30 && g > b + 30))
				{
					pixel_dst = data->addr + (screen_y * data->line_len
							+ screen_x * (data->bpp / 8));
					*(unsigned int *)pixel_dst = color;
				}
			}
			hx++;
		}
		hy++;
	}
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

int	key_press(int keycode, t_mlx *data)
{
	printf("Touche appuyée: %d\n", keycode); // AJOUTE ÇA
	if (keycode == 65307)
		close_window(data);
	if (keycode < 65536)
		data->keys[keycode] = 1;
	return (0);
}

int	key_release(int keycode, t_mlx *data)
{
	if (keycode < 65536)
		data->keys[keycode] = 0; // Marquer la touche comme relâchée
	return (0);
}

int	game_loop(t_mlx *data)
{
	double	move_speed;
	double	rot_speed;
	double	new_x;
	double	new_y;
	int		moved;
	double	dir_x;
	double	dir_y;
	double	margin;
	int		can_move;

	move_speed = 0.02;
	rot_speed = 0.5;
	new_x = data->player.player_x;
	new_y = data->player.player_y;
	moved = 0;
	// Calculer les vecteurs de direction
	dir_x = cos(data->player.dir_angle * M_PI / 180.0);
	dir_y = sin(data->player.dir_angle * M_PI / 180.0);
	// W - Avancer dans la direction de vue
	if (data->keys[119])
	{
		new_x += dir_x * move_speed;
		new_y += dir_y * move_speed;
		moved = 1;
	}
	// S - Reculer (direction opposée)
	if (data->keys[115])
	{
		new_x -= dir_x * move_speed;
		new_y -= dir_y * move_speed;
		moved = 1;
	}
	// A - Strafe gauche (perpendiculaire à gauche)
	if (data->keys[97])
	{
		new_x += dir_y * move_speed; // Perpendiculaire
		new_y -= dir_x * move_speed;
		moved = 1;
	}
	// D - Strafe droite (perpendiculaire à droite)
	if (data->keys[100])
	{
		new_x -= dir_y * move_speed; // Perpendiculaire
		new_y += dir_x * move_speed;
		moved = 1;
	}
	// Rotation (comme avant)
	if (data->keys[107]) // Flèche gauche
	{
		data->player.dir_angle -= rot_speed;
		if (data->player.dir_angle < 0)
			data->player.dir_angle += 360.0;
		moved = 1;
	}
	if (data->keys[108]) // Flèche droite
	{
		data->player.dir_angle += rot_speed;
		if (data->player.dir_angle >= 360.0)
			data->player.dir_angle -= 360.0;
		moved = 1;
	}
	// Collisions et render (comme avant)
	margin = 0.2;
	can_move = 1;
	if (data->map.grid[(int)(new_y - margin)][(int)(new_x - margin)] == '1'
		|| data->map.grid[(int)(new_y - margin)][(int)(new_x + margin)] == '1'
		|| data->map.grid[(int)(new_y + margin)][(int)(new_x - margin)] == '1'
		|| data->map.grid[(int)(new_y + margin)][(int)(new_x + margin)] == '1')
	{
		can_move = 0;
	}
	if (moved && can_move)
	{
		data->player.player_x = new_x;
		data->player.player_y = new_y;
		render_3d(data);
	}
	else if (moved)
	{
		render_3d(data);
	}
	return (0);
}

void	draw_minimap(t_mlx *data)
{
	int	scale;
	int	offset_x;
	int	offset_y;
	int	row;
	int	col;
	int	color;
	int	player_pixel_x;
	int	player_pixel_y;

	scale = 10;
	offset_x = 10;
	offset_y = 10;
	// Fond noir pour la minimap
	row = 0;
	while (row < data->map.height)
	{
		col = 0;
		while (col < data->map.width)
		{
			if (data->map.grid[row][col] == '1')
				color = 0x00245969;
			else
				color = 0x00000000;
			draw_square(data, offset_x + col * scale, offset_y + row * scale,
				scale, color);
			col++;
		}
		row++;
	}
	// Dessiner le joueur - MODIFIÉ ICI
	player_pixel_x = offset_x + (int)(data->player.player_x * scale) - 2;
	player_pixel_y = offset_y + (int)(data->player.player_y * scale) - 2;
	draw_square(data, player_pixel_x, player_pixel_y, 4, 0x00FF0000);
}

int	main(int ac, char **av)
{
	t_mlx data;

	if (ac != 2)
		return (printf("ac != 2\n"), 1);

	parse_map(&data, av);

	int i = 0;
	while (i < 65536)
	{
		data.keys[i] = 0;
		i++;
	}

	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);

	// AJOUTE CETTE LIGNE - Créer la fenêtre
	data.win = mlx_new_window(data.mlx, 800, 600, "Cub3D");
	if (!data.win)
		return (1);

	// Charger texture mur
	data.wall_texture = mlx_xpm_file_to_image(data.mlx,
			"../sprites/mur_bleu.xpm", &data.wall_w, &data.wall_h);
	if (!data.wall_texture)
		return (printf("Erreur: texture mur\n"), 1);
	data.wall_addr = mlx_get_data_addr(data.wall_texture, &data.wall_bpp,
			&data.wall_line_len, &data.wall_endian);

	data.hands_texture = mlx_xpm_file_to_image(data.mlx, "../sprites/hands.xpm",
			&data.hands_w, &data.hands_h);
	if (!data.hands_texture)
		return (printf("Erreur: texture mains\n"), 1);
	data.hands_addr = mlx_get_data_addr(data.hands_texture, &data.hands_bpp,
			&data.hands_line_len, &data.hands_endian);
	// Créer l'image de rendu
	data.img = mlx_new_image(data.mlx, 800, 600);
	if (!data.img)
		return (1);
	data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_len,
			&data.endian);

	render_3d(&data);

	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, game_loop, &data);
	mlx_loop(data.mlx);
	return (0);
}
