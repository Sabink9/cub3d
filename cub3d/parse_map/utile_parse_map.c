/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile_parse_map.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:59:53 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 15:12:32 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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

static int	ft_atoi_color(char *str)
{
	int	n;

	n = 0;
	while (*str == ' ' || *str == '\t')
		str++;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + (*str++ - '0');
	return (n);
}

int	parse_color(char *str, unsigned int *color)
{
	int	r;
	int	g;
	int	b;

	while (*str == ' ' || *str == '\t')
		str++;
	r = ft_atoi_color(str);
	while (*str && *str != ',')
		str++;
	if (!*str)
		return (0);
	g = ft_atoi_color(++str);
	while (*str && *str != ',')
		str++;
	if (!*str)
		return (0);
	b = ft_atoi_color(++str);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	*color = ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
	return (1);
}
