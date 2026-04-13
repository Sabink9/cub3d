/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:41:33 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:10:42 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	drain_fd(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}

static int	parse_headers(t_mlx *data, int fd, int *flags)
{
	char	*line;
	int		ret;

	line = get_next_line(fd);
	while (line && *flags != 63)
	{
		strip_newline(line);
		ret = parse_header_line(data, line, flags);
		free(line);
		if (ret == -1)
			return (drain_fd(fd), 0);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	drain_fd(fd);
	return (1);
}

void	free_data(t_mlx *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->tex[i].path)
		{
			free(data->tex[i].path);
			data->tex[i].path = NULL;
		}
		i++;
	}
	if (data->map.grid)
	{
		i = 0;
		while (data->map.grid[i])
			free(data->map.grid[i++]);
		free(data->map.grid);
		data->map.grid = NULL;
	}
}

static int	init_map(t_mlx *data, char *path)
{
	int	fd;
	int	height_flags;

	init_data_null(data);
	height_flags = 0;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (ft_putstr("Error: cannot open file\n"), 0);
	data->map.height = count_map_lines(fd, &height_flags);
	close(fd);
	if (height_flags != 63)
		return (ft_putstr("Error: missing identifiers in .cub file\n"), 0);
	if (data->map.height == 0)
		return (ft_putstr("Error: no map found\n"), 0);
	return (1);
}

int	parse_map(t_mlx *data, char **av)
{
	int	fd;
	int	flags;

	if (!init_map(data, av[1]))
		return (0);
	flags = 0;
	fd = open(av[1], O_RDONLY);
	if (!parse_headers(data, fd, &flags))
		return (close(fd), free_data(data), 0);
	close(fd);
	if (flags != 63)
		return (free_data(data),
			ft_putstr("Error: missing identifiers in .cub file\n"), 0);
	fd = open(av[1], O_RDONLY);
	fill_grid(&data->map, fd);
	close(fd);
	if (!validate_map(data, av[1]))
		return (free_data(data), 0);
	find_player(&data->map, &data->player);
	return (1);
}
