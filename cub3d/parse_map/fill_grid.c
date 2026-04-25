/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:53:48 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/25 00:00:00 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line[i] || line[i] == '\n')
		return (0);
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && line[i] != 'N'
			&& line[i] != 'S' && line[i] != 'E' && line[i] != 'W'
			&& line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

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

static int	process_line(t_map *map, char *line, t_parse *p)
{
	strip_newline(line);
	if (line[0] == '#')
		return (ft_putstr("Error: comment not allowed in .cub file\n"), -1);
	if (!p->in_map)
		has_all_headers(line, &p->flags);
	if (p->flags == 63 && is_map_line(line))
		p->in_map = 1;
	if (p->in_map && line[0] == '\0')
		return (ft_putstr("Error: empty line in map\n"), -1);
	if (p->in_map && line[0] != '\0')
	{
		map->grid[p->y] = ft_strdup(line);
		if (ft_strlen(map->grid[p->y]) > map->width)
			map->width = ft_strlen(map->grid[p->y]);
		p->y++;
	}
	return (0);
}

static int	fill_rows(t_map *map, int fd)
{
	char	*line;
	t_parse	p;

	p.flags = 0;
	p.in_map = 0;
	p.y = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (process_line(map, line, &p) == -1)
		{
			free(line);
			map->grid[p.y] = NULL;
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	map->grid[p.y] = NULL;
	return (0);
}

int	fill_grid(t_map *map, int fd)
{
	int	y;

	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		exit(1);
	if (fill_rows(map, fd) == -1)
		return (-1);
	y = 0;
	while (map->grid[y])
	{
		pad_row(&map->grid[y], map->width);
		y++;
	}
	return (0);
}
