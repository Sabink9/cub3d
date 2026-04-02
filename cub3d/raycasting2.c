#include "cub3d.h"

static void	init_ray_result(t_ray *result, double rx, double ry, int out)
{
	if (out)
	{
		result->distance = 999.0;
		result->hit_x = rx;
		result->hit_y = ry;
		result->side = 0;
	}
}

static int	determine_side(double ray_x)
{
	double	frac_x;

	frac_x = ray_x - floor(ray_x);
	if (frac_x < 0.01 || frac_x > 0.99)
		return (0);
	return (1);
}

static void	calc_hit_result(t_mlx *data, t_ray *result, double rx, double ry, double angle)
{
	double	dx;
	double	dy;
	double	distance;
	double	angle_diff;

	dx = rx - data->player.player_x;
	dy = ry - data->player.player_y;
	distance = sqrt(dx * dx + dy * dy);
	angle_diff = angle - data->player.dir_angle;
	distance = distance * cos(angle_diff * M_PI / 180.0);
	if (distance < 0.1)
		distance = 0.1;
	result->distance = distance;
	result->hit_x = rx;
	result->hit_y = ry;
	result->side = determine_side(rx);
}

t_ray	cast_ray(t_mlx *data, double angle)
{
	t_ray	result;
	double	ray_pos[2];
	double	ray_dir[2];
	int		map_pos[2];

	ray_pos[0] = data->player.player_x;
	ray_pos[1] = data->player.player_y;
	ray_dir[0] = cos(angle * M_PI / 180.0);
	ray_dir[1] = sin(angle * M_PI / 180.0);
	while (1)
	{
		ray_pos[0] += ray_dir[0] * 0.01;
		ray_pos[1] += ray_dir[1] * 0.01;
		map_pos[0] = (int)ray_pos[0];
		map_pos[1] = (int)ray_pos[1];
		if (map_pos[0] < 0 || map_pos[0] >= data->map.width || map_pos[1] < 0 || map_pos[1] >= data->map.height)
			return (init_ray_result(&result, ray_pos[0], ray_pos[1], 1), result);
		if (data->map.grid[map_pos[1]][map_pos[0]] == '1')
		{
			calc_hit_result(data, &result, ray_pos[0], ray_pos[1], angle);
			return (result);
		}
	}
}

static int	calc_tex_x(t_ray ray, int wall_w)
{
	double	wall_x;
	int		tex_x;

	if (ray.side == 0)
		wall_x = ray.hit_y;
	else
		wall_x = ray.hit_x;
	wall_x = wall_x - floor(wall_x);
	tex_x = (int)(wall_x * wall_w);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= wall_w)
		tex_x = wall_w - 1;
	return (tex_x);
}

static void	draw_column_pixel(t_mlx *data, int x, int y, int tex_x, int *bounds)
{
	char	*pixel;
	int		tex_y;
	char	*tex_pixel;

	pixel = data->addr + (y * data->line_len + x * (data->bpp / 8));
	if (y < bounds[0])
		*(unsigned int *)pixel = 0x0087CEEB;
	else if (y >= bounds[0] && y <= bounds[1])
	{
		tex_y = ((y - bounds[0]) * data->wall_h) / bounds[2];
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= data->wall_h)
			tex_y = data->wall_h - 1;
		tex_pixel = data->wall_addr + (tex_y * data->wall_line_len + tex_x * (data->wall_bpp / 8));
		*(unsigned int *)pixel = *(unsigned int *)tex_pixel;
	}
	else
		*(unsigned int *)pixel = 0x00808080;
}

static void	draw_column(t_mlx *data, int x, t_ray ray)
{
	int	wall_height;
	int	bounds[3];
	int	tex_x;
	int	y;

	wall_height = (int)(1200 / ray.distance);
	if (wall_height > 3000)
		wall_height = 3000;
	bounds[0] = (600 / 2) - (wall_height / 2);
	bounds[1] = (600 / 2) + (wall_height / 2);
	bounds[2] = wall_height;
	tex_x = calc_tex_x(ray, data->wall_w);
	y = 0;
	while (y < 600)
	{
		draw_column_pixel(data, x, y, tex_x, bounds);
		y++;
	}
}

void	render_3d(t_mlx *data)
{
	int		x;
	double	angle_offset;
	double	ray_angle;
	t_ray	ray;

	x = 0;
	while (x < 800)
	{
		angle_offset = (60.0 / 800) * x - (60.0 / 2.0);
		ray_angle = data->player.dir_angle + angle_offset;
		ray = cast_ray(data, ray_angle);
		draw_column(data, x, ray);
		x++;
	}
	draw_minimap(data);
	draw_hands(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
