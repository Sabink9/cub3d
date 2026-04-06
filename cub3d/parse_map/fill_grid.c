/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:53:48 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:12:54 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	pad_row(char **row, int target_width)
{
	int		len;
	char	*padded;
	int		i;

	len = ft_strlen(*row);
	if (len >= target_width)
		return ;
	padded = malloc(target_width + 1);
	if (!padded)
		return ;
	i = 0;
	while (i < len)
	{
		padded[i] = (*row)[i];
		i++;
	}
	while (i < target_width)
		padded[i++] = ' ';
	padded[i] = '\0';
	free(*row);
	*row = padded;
}

static void	process_line(t_map *map, char *line, t_parse *p)
{
	strip_newline(line);
	if (!p->in_map)
		has_all_headers(line, &p->flags);
	if (p->flags == 63 && is_map_line(line))
		p->in_map = 1;
	if (p->in_map && line[0] != '\0')
	{
		map->grid[p->y] = ft_strdup(line);
		if (ft_strlen(map->grid[p->y]) > map->width)
			map->width = ft_strlen(map->grid[p->y]);
		p->y++;
	}
}

static void	fill_rows(t_map *map, int fd)
{
	char	*line;
	t_parse	p;

	p.flags = 0;
	p.in_map = 0;
	p.y = 0;
	line = get_next_line(fd);
	while (line)
	{
		process_line(map, line, &p);
		free(line);
		line = get_next_line(fd);
	}
	map->grid[p.y] = NULL;
}

void	fill_grid(t_map *map, int fd)
{
	int		y;

	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		exit(1);
	fill_rows(map, fd);
	y = 0;
	while (map->grid[y])
	{
		pad_row(&map->grid[y], map->width);
		y++;
	}
}
