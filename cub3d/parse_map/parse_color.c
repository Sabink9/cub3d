/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
