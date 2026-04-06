/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_hands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:23:59 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:53:29 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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

static int	init_hand(t_mlx *data, t_hand *h)
{
	h->tex_x = h->dx * data->hands_w / h->info[0];
	h->tex_y = h->dy * data->hands_h / h->info[1];
	h->screen_x = (WIN_W - h->info[0]) / 2 + h->dx;
	h->screen_y = WIN_H - h->info[1] + h->dy;
	if (h->screen_x < 0 || h->screen_x >= WIN_W
		|| h->screen_y < 0 || h->screen_y >= WIN_H)
		return (0);
	return (1);
}

static void	draw_hand_pixel(t_mlx *data, int dx, int dy, int *info)
{
	t_hand	h;

	h.dx = dx;
	h.dy = dy;
	h.info = info;
	if (!init_hand(data, &h))
		return ;
	h.pixel_src = data->hands_addr + (h.tex_y * data->hands_line_len
			+ h.tex_x * (data->hands_bpp / 8));
	h.color = *(unsigned int *)h.pixel_src;
	if (!is_green_pixel(h.color))
	{
		h.pixel_dst = data->addr + (h.screen_y * data->line_len
				+ h.screen_x * (data->bpp / 8));
		*(unsigned int *)h.pixel_dst = h.color;
	}
}

void	draw_hands(t_mlx *data)
{
	int	info[2];
	int	dy;
	int	dx;

	info[0] = data->hands_w * WIN_W / 800;
	info[1] = data->hands_h * WIN_H / 600;
	dy = 0;
	while (dy < info[1])
	{
		dx = 0;
		while (dx < info[0])
		{
			draw_hand_pixel(data, dx, dy, info);
			dx++;
		}
		dy++;
	}
}
