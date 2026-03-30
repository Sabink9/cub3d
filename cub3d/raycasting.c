#include "cub3d.h"

t_ray	cast_ray(t_mlx *data, double angle)
{
	t_ray	result;
	double	ray_x;
	double	ray_y;
	double	ray_dir_x;
	double	ray_dir_y;
	double	step;
	int		map_x;
	int		map_y;
	double	dx;
	double	dy;
	double	distance;
	double	angle_diff;
	double	frac_x;

	ray_x = data->player.player_x;
	ray_y = data->player.player_y;
	ray_dir_x = cos(angle * M_PI / 180.0);
	ray_dir_y = sin(angle * M_PI / 180.0);
	step = 0.01;
	while (1)
	{
		ray_x += ray_dir_x * step;
		ray_y += ray_dir_y * step;
		map_x = (int)ray_x;
		map_y = (int)ray_y;
		if (map_x < 0 || map_x >= data->map.width || map_y < 0
			|| map_y >= data->map.height)
		{
			result.distance = 999.0;
			result.hit_x = ray_x;
			result.hit_y = ray_y;
			result.side = 0;
			return (result);
		}
		if (data->map.grid[map_y][map_x] == '1')
		{
			dx = ray_x - data->player.player_x;
			dy = ray_y - data->player.player_y;
			distance = sqrt(dx * dx + dy * dy);
			angle_diff = angle - data->player.dir_angle;
			distance = distance * cos(angle_diff * M_PI / 180.0);
			if (distance < 0.1)
				distance = 0.1;
			result.distance = distance;
			result.hit_x = ray_x;
			result.hit_y = ray_y;
			// Déterminer si c'est un mur vertical ou horizontal
			frac_x = ray_x - floor(ray_x);
			if (frac_x < 0.01 || frac_x > 0.99)
				result.side = 0; // Mur vertical
			else
				result.side = 1; // Mur horizontal
			return (result);
		}
	}
}

void render_3d(t_mlx *data)
{
    int x = 0;
    double fov = 60.0;
    int screen_width = 800;

    while (x < screen_width)
    {
        double angle_offset = (fov / screen_width) * x - (fov / 2.0);
        double ray_angle = data->player.dir_angle + angle_offset;

        t_ray ray = cast_ray(data, ray_angle);
        double distance = ray.distance;

        int wall_height = (int)(1200 / distance);
        if (wall_height > 3000)
            wall_height = 3000;

        int wall_start = (600 / 2) - (wall_height / 2);
        int wall_end = (600 / 2) + (wall_height / 2);

        // Calculer tex_x selon le point d'impact
        double wall_x;
        if (ray.side == 0)  // Mur vertical
            wall_x = ray.hit_y;
        else  // Mur horizontal
            wall_x = ray.hit_x;

        wall_x = wall_x - floor(wall_x);  // Partie décimale
        int tex_x = (int)(wall_x * data->wall_w);
        if (tex_x < 0)
            tex_x = 0;
        if (tex_x >= data->wall_w)
            tex_x = data->wall_w - 1;

        int y = 0;
        while (y < 600)
        {
            char *pixel = data->addr + (y * data->line_len + x * (data->bpp / 8));

            if (y < wall_start)
            {
                *(unsigned int *)pixel = 0x0087CEEB;  // Ciel bleu
            }
            else if (y >= wall_start && y <= wall_end)
            {
                int tex_y = ((y - wall_start) * data->wall_h) / wall_height;
                
                if (tex_y < 0)
                    tex_y = 0;
                if (tex_y >= data->wall_h)
                    tex_y = data->wall_h - 1;

                char *tex_pixel = data->wall_addr + (tex_y * data->wall_line_len + tex_x * (data->wall_bpp / 8));
                *(unsigned int *)pixel = *(unsigned int *)tex_pixel;
            }
            else
            {
                *(unsigned int *)pixel = 0x00808080;  // Sol gris
            }

            y++;
        }
        x++;
    }

    draw_minimap(data);
    draw_hands(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}