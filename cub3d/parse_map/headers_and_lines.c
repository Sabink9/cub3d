/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers_and_lines.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:56:16 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:13:16 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static char	*trim_path(char *str)
{
	int		len;
	char	*path;
	int		i;

	while (*str == ' ' || *str == '\t')
		str++;
	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ' || str[len
				- 1] == '\t' || str[len - 1] == '\r'))
		len--;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	i = 0;
	while (i < len)
	{
		path[i] = str[i];
		i++;
	}
	path[i] = '\0';
	return (path);
}

int	parse_header_line(t_mlx *data, char *line, int *flags)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (free(data->tex[NORTH].path),
			data->tex[NORTH].path = trim_path(line + 3), *flags |= 1, 1);
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (free(data->tex[SOUTH].path),
			data->tex[SOUTH].path = trim_path(line + 3), *flags |= 2, 1);
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (free(data->tex[EAST].path),
			data->tex[EAST].path = trim_path(line + 3), *flags |= 4, 1);
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (free(data->tex[WEST].path),
			data->tex[WEST].path = trim_path(line + 3), *flags |= 8, 1);
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
	{
		if (!parse_color(line + 2, &data->floor_color))
			return (ft_putstr("Error: invalid color value (must be 0-255)\n"), -1);
		return (*flags |= 16, 1);
	}
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
	{
		if (!parse_color(line + 2, &data->ceil_color))
			return (ft_putstr("Error: invalid color value (must be 0-255)\n"), -1);
		return (*flags |= 32, 1);
	}
	return (0);
}

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

int	has_all_headers(char *line, int *flags)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		*flags |= 1;
	else if (ft_strncmp(line, "SO ", 3) == 0)
		*flags |= 2;
	else if (ft_strncmp(line, "EA ", 3) == 0)
		*flags |= 4;
	else if (ft_strncmp(line, "WE ", 3) == 0)
		*flags |= 8;
	else if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
		*flags |= 16;
	else if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
		*flags |= 32;
	return (*flags == 63);
}

int	count_map_lines(int fd, int *flags)
{
	char	*line;
	int		count;
	int		in_map;

	count = 0;
	in_map = 0;
	line = get_next_line(fd);
	while (line)
	{
		strip_newline(line);
		if (!in_map)
			has_all_headers(line, flags);
		if (*flags == 63 && is_map_line(line))
			in_map = 1;
		if (in_map && line[0] != '\0')
			count++;
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}
