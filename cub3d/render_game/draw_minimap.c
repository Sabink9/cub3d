/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:21:26 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:44:57 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	draw_pixel(t_mlx *data, int pixel_x, int pixel_y, int color)
{
	char	*pixel;

	if (pixel_x >= 0 && pixel_x < WIN_W && pixel_y >= 0 && pixel_y < WIN_H)
	{
		pixel = data->addr + (pixel_y * data->line_len + pixel_x * (data->bpp
					/ 8));
		*(unsigned int *)pixel = color;
	}
}

static void	draw_square(t_mlx *data, t_square sq)
{
	int	i;
	int	j;

	i = 0;
	while (i < sq.size)
	{
		j = 0;
		while (j < sq.size)
		{
			draw_pixel(data, sq.x + j, sq.y + i, sq.color);
			j++;
		}
		i++;
	}
}

static void	draw_minimap_grid(t_mlx *data, int scale, int *offset)
{
	int			row;
	int			col;
	t_square	sq;

	row = 0;
	sq.size = scale;
	while (row < data->map.height)
	{
		col = 0;
		while (col < data->map.width)
		{
			if (data->map.grid[row][col] == '1')
				sq.color = 0x00245969;
			else
				sq.color = 0x00000000;
			sq.x = offset[0] + col * scale;
			sq.y = offset[1] + row * scale;
			draw_square(data, sq);
			col++;
		}
		row++;
	}
}

void	draw_minimap(t_mlx *data)
{
	int			scale;
	int			offset[2];
	t_square	sq;

	scale = 10;
	offset[0] = 10;
	offset[1] = 10;
	draw_minimap_grid(data, scale, offset);
	sq.x = offset[0] + (int)(data->player.player_x * scale) - 2;
	sq.y = offset[1] + (int)(data->player.player_y * scale) - 2;
	sq.size = 4;
	sq.color = 0x00FF0000;
	draw_square(data, sq);
}
