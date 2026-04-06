/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_cast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:41:25 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 16:15:38 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static t_ray	cast_ray(t_mlx *data, double angle)
{
	t_dda	d;
	int		side;

	init_dda(data, angle, &d);
	side = 0;
	while (1)
	{
		dda_step(&d, &side);
		if (d.map[0] < 0 || d.map[0] >= data->map.width
			|| d.map[1] < 0 || d.map[1] >= data->map.height)
			return (out_of_bounds());
		if (data->map.grid[d.map[1]][d.map[0]] == '1')
			return (hit_wall(data, &d, side));
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

static void	draw_column_pixel(t_mlx *data, int x, int y, t_column *col)
{
	char	*pixel;
	int		tex_y;
	char	*tex_pixel;
	t_tex	*tex;

	pixel = data->addr + (y * data->line_len + x * (data->bpp / 8));
	if (y < col->draw_start)
		*(unsigned int *)pixel = data->ceil_color;
	else if (y <= col->draw_end)
	{
		tex = &data->tex[col->side];
		tex_y = ((y - col->draw_start) * tex->h) / col->wall_height;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->h)
			tex_y = tex->h - 1;
		tex_pixel = tex->addr + (tex_y * tex->line_len
				+ col->tex_x * (tex->bpp / 8));
		*(unsigned int *)pixel = *(unsigned int *)tex_pixel;
	}
	else
		*(unsigned int *)pixel = data->floor_color;
}

static void	draw_column(t_mlx *data, int x, t_ray ray)
{
	t_column	col;
	int			y;

	col.wall_height = (int)((WIN_H * 1.5) / ray.distance);
	if (col.wall_height > WIN_H * 5)
		col.wall_height = WIN_H * 5;
	col.draw_start = (WIN_H / 2) - (col.wall_height / 2);
	col.draw_end = (WIN_H / 2) + (col.wall_height / 2);
	col.side = ray.side;
	col.tex_x = calc_tex_x(ray, data->tex[ray.side].w);
	y = 0;
	while (y < WIN_H)
	{
		draw_column_pixel(data, x, y, &col);
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
