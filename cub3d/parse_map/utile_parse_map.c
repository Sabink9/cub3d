/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile_parse_map.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:59:53 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/20 16:19:51 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	if (!str)
		return ;
	write(1, str, ft_strlen(str));
}

void	strip_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (!s1[i])
			return (0);
		i++;
	}
	return (0);
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
