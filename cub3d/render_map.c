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
			if (pixel_x >= 0 && pixel_x < WIN_W
				&& pixel_y >= 0 && pixel_y < WIN_H)
			{
				pixel = data->addr + (pixel_y * data->line_len
						+ pixel_x * (data->bpp / 8));
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
}

static int	is_green_pixel(unsigned int color)
{
	int	r;
	int	g;
	int	b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	if (g > 80 && g > r + 30 && g > b + 30)
		return (1);
	return (0);
}

static void	draw_hand_pixel(t_mlx *data, int hx, int hy, int *start)
{
	int				screen_x;
	int				screen_y;
	char			*pixel_src;
	unsigned int	color;
	char			*pixel_dst;

	screen_x = start[0] + hx;
	screen_y = start[1] + hy;
	if (screen_x >= 0 && screen_x < WIN_W
		&& screen_y >= 0 && screen_y < WIN_H)
	{
		pixel_src = data->hands_addr
			+ (hy * data->hands_line_len + hx * (data->hands_bpp / 8));
		color = *(unsigned int *)pixel_src;
		if (!is_green_pixel(color))
		{
			pixel_dst = data->addr + (screen_y * data->line_len
					+ screen_x * (data->bpp / 8));
			*(unsigned int *)pixel_dst = color;
		}
	}
}

void	draw_hands(t_mlx *data)
{
	int	start[2];
	int	hy;
	int	hx;

	start[1] = WIN_H - data->hands_h;
	start[0] = (WIN_W - data->hands_w) / 2;
	hy = 0;
	while (hy < data->hands_h)
	{
		hx = 0;
		while (hx < data->hands_w)
		{
			draw_hand_pixel(data, hx, hy, start);
			hx++;
		}
		hy++;
	}
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

static void	handle_movement(t_mlx *data, double *new_x, double *new_y,
	int *moved)
{
	double	move_speed;
	double	dir_x;
	double	dir_y;

	move_speed = 0.05;
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

static void	handle_strafe(t_mlx *data, double *new_x, double *new_y,
	int *moved)
{
	double	move_speed;
	double	dir_x;
	double	dir_y;

	move_speed = 0.05;
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

static void	handle_rotation(t_mlx *data, int *moved)
{
	double	rot_speed;

	rot_speed = 1.5;
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

static int	check_collision(t_mlx *data, double new_x, double new_y)
{
	double	margin;
	int		x_min;
	int		x_max;
	int		y_min;
	int		y_max;

	margin = 0.55;
	x_min = (int)(new_x - margin);
	x_max = (int)(new_x + margin);
	y_min = (int)(new_y - margin);
	y_max = (int)(new_y + margin);
	if (x_min < 0 || x_max >= data->map.width
		|| y_min < 0 || y_max >= data->map.height)
		return (0);
	if (data->map.grid[y_min][x_min] == '1')
		return (0);
	if (data->map.grid[y_min][x_max] == '1')
		return (0);
	if (data->map.grid[y_max][x_min] == '1')
		return (0);
	if (data->map.grid[y_max][x_max] == '1')
		return (0);
	return (1);
}

int	game_loop(t_mlx *data)
{
	double	new_x;
	double	new_y;
	int		moved;

	new_x = data->player.player_x;
	new_y = data->player.player_y;
	moved = 0;
	handle_movement(data, &new_x, &new_y, &moved);
	handle_strafe(data, &new_x, &new_y, &moved);
	handle_rotation(data, &moved);
	if (moved && check_collision(data, new_x, new_y))
	{
		data->player.player_x = new_x;
		data->player.player_y = new_y;
	}
	if (moved)
		render_3d(data);
	return (0);
}

static void	draw_minimap_grid(t_mlx *data, int scale, int *offset)
{
	int	row;
	int	col;
	int	color;

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
			draw_square(data, offset[0] + col * scale,
				offset[1] + row * scale, scale, color);
			col++;
		}
		row++;
	}
}

void	draw_minimap(t_mlx *data)
{
	int	scale;
	int	offset[2];
	int	player_pixel_x;
	int	player_pixel_y;

	scale = 10;
	offset[0] = 10;
	offset[1] = 10;
	draw_minimap_grid(data, scale, offset);
	player_pixel_x = offset[0] + (int)(data->player.player_x * scale) - 2;
	player_pixel_y = offset[1] + (int)(data->player.player_y * scale) - 2;
	draw_square(data, player_pixel_x, player_pixel_y, 4, 0x00FF0000);
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

static void	init_keys(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 65536)
	{
		data->keys[i] = 0;
		i++;
	}
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
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
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
