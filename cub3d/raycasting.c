#include "cub3d.h"

static void	init_dda(t_mlx *data, double rdx, double rdy,
	double *side_dist, double *delta_dist, int *step, int *map)
{
	map[0] = (int)data->player.player_x;
	map[1] = (int)data->player.player_y;
	delta_dist[0] = rdx != 0 ? fabs(1.0 / rdx) : 1e30;
	delta_dist[1] = rdy != 0 ? fabs(1.0 / rdy) : 1e30;
	if (rdx < 0)
	{
		step[0] = -1;
		side_dist[0] = (data->player.player_x - map[0]) * delta_dist[0];
	}
	else
	{
		step[0] = 1;
		side_dist[0] = (map[0] + 1.0 - data->player.player_x) * delta_dist[0];
	}
	if (rdy < 0)
	{
		step[1] = -1;
		side_dist[1] = (data->player.player_y - map[1]) * delta_dist[1];
	}
	else
	{
		step[1] = 1;
		side_dist[1] = (map[1] + 1.0 - data->player.player_y) * delta_dist[1];
	}
}

t_ray	cast_ray(t_mlx *data, double angle)
{
	t_ray	result;
	double	rdx;
	double	rdy;
	double	side_dist[2];
	double	delta_dist[2];
	int		step[2];
	int		map[2];
	int		side;
	double	perp_dist;

	rdx = cos(angle * M_PI / 180.0);
	rdy = sin(angle * M_PI / 180.0);
	init_dda(data, rdx, rdy, side_dist, delta_dist, step, map);
	side = 0;
	while (1)
	{
		if (side_dist[0] < side_dist[1])
		{
			side_dist[0] += delta_dist[0];
			map[0] += step[0];
			side = 0;
		}
		else
		{
			side_dist[1] += delta_dist[1];
			map[1] += step[1];
			side = 1;
		}
		if (map[0] < 0 || map[0] >= data->map.width
			|| map[1] < 0 || map[1] >= data->map.height)
		{
			result.distance = 999.0;
			result.hit_x = 0;
			result.hit_y = 0;
			result.side = NORTH;
			return (result);
		}
		if (data->map.grid[map[1]][map[0]] == '1')
		{
			if (side == 0)
			{
				perp_dist = side_dist[0] - delta_dist[0];
				result.hit_y = data->player.player_y + perp_dist * rdy;
				result.hit_x = 0;
				if (step[0] > 0)
					result.side = WEST;
				else
					result.side = EAST;
			}
			else
			{
				perp_dist = side_dist[1] - delta_dist[1];
				result.hit_x = data->player.player_x + perp_dist * rdx;
				result.hit_y = 0;
				if (step[1] > 0)
					result.side = NORTH;
				else
					result.side = SOUTH;
			}
			if (perp_dist < 0.1)
				perp_dist = 0.1;
			result.distance = perp_dist;
			return (result);
		}
	}
}

static int	calc_tex_x(t_ray ray, int wall_w)
{
	double	wall_x;
	int		tex_x;

	if (ray.side == NORTH || ray.side == SOUTH)
		wall_x = ray.hit_x - (int)ray.hit_x;
	else
		wall_x = ray.hit_y - (int)ray.hit_y;
	if (wall_x < 0)
		wall_x += 1.0;
	tex_x = (int)(wall_x * wall_w / 4);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= wall_w)
		tex_x = wall_w - 1;
	return (tex_x);
}

static void	draw_column_pixel(t_mlx *data, int x, int y,
	int tex_x, int *bounds)
{
	char			*pixel;
	int				tex_y;
	char			*tex_pixel;
	t_tex			*tex;

	pixel = data->addr + (y * data->line_len + x * (data->bpp / 8));
	if (y < bounds[0])
		*(unsigned int *)pixel = data->ceil_color;
	else if (y <= bounds[1])
	{
		tex = &data->tex[bounds[3]];
		tex_y = ((y - bounds[0]) * tex->h) / bounds[2];
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->h)
			tex_y = tex->h - 1;
		tex_pixel = tex->addr + (tex_y * tex->line_len
				+ tex_x * (tex->bpp / 8));
		*(unsigned int *)pixel = *(unsigned int *)tex_pixel;
	}
	else
		*(unsigned int *)pixel = data->floor_color;
}

static void	draw_column(t_mlx *data, int x, t_ray ray)
{
	int	wall_height;
	int	bounds[4];
	int	tex_x;
	int	y;

	wall_height = (int)((WIN_H * 1.5) / ray.distance);
	if (wall_height > WIN_H * 5)
		wall_height = WIN_H * 5;
	bounds[0] = (WIN_H / 2) - (wall_height / 2);
	bounds[1] = (WIN_H / 2) + (wall_height / 2);
	bounds[2] = wall_height;
	bounds[3] = ray.side;
	tex_x = calc_tex_x(ray, data->tex[ray.side].w);
	y = 0;
	while (y < WIN_H)
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
	while (x < WIN_W)
	{
		angle_offset = (FOV / WIN_W) * x - (FOV / 2.0);
		ray_angle = data->player.dir_angle + angle_offset;
		ray = cast_ray(data, ray_angle);
		draw_column(data, x, ray);
		x++;
	}
	draw_minimap(data);
	draw_hands(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
