/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers_and_lines.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:56:16 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/25 00:00:00 by saciurus         ###   ########.fr       */
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

static int	match_dir(char *line, int *idx, int *bit)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (*idx = NORTH, *bit = 1, 1);
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (*idx = SOUTH, *bit = 2, 1);
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (*idx = EAST, *bit = 4, 1);
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (*idx = WEST, *bit = 8, 1);
	return (0);
}

static int	parse_tex_line(t_mlx *data, char *line, int *flags)
{
	int	idx;
	int	bit;

	idx = 0;
	bit = 0;
	if (!match_dir(line, &idx, &bit))
		return (-1);
	if (*flags & bit)
		return (ft_putstr("Error: duplicate texture identifier\n"), -1);
	free(data->tex[idx].path);
	data->tex[idx].path = trim_path(line + 3);
	*flags |= bit;
	return (1);
}

int	parse_header_line(t_mlx *data, char *line, int *flags)
{
	int	res;

	if (line[0] == '#')
		return (ft_putstr("Error: comment not allowed in .cub file\n"), -1);
	res = parse_tex_line(data, line, flags);
	if (res != -1)
		return (res);
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
	{
		res = parse_color(line + 2, &data->floor_color);
		if (res != 1)
			return (ft_putstr(res ? "Error: too many RGB values\n"
					: "Error: invalid floor color\n"), -1);
		return (*flags |= 16, 1);
	}
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
	{
		res = parse_color(line + 2, &data->ceil_color);
		if (res != 1)
			return (ft_putstr(res ? "Error: too many RGB values\n"
					: "Error: invalid ceiling color\n"), -1);
		return (*flags |= 32, 1);
	}
	return (0);
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
