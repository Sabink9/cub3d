/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/25 00:00:00 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	ft_atoi_color(char *str)
{
	int	n;

	n = 0;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + (*str++ - '0');
	return (n);
}

static int	parse_component(char **str, int *val)
{
	while (**str == ' ' || **str == '\t')
		(*str)++;
	if (!(**str >= '0' && **str <= '9'))
		return (0);
	*val = ft_atoi_color(*str);
	while (**str >= '0' && **str <= '9')
		(*str)++;
	while (**str == ' ' || **str == '\t')
		(*str)++;
	return (1);
}

int	parse_color(char *str, unsigned int *color)
{
	int	r;
	int	g;
	int	b;

	if (!parse_component(&str, &r) || *str != ',')
		return (0);
	str++;
	if (!parse_component(&str, &g) || *str != ',')
		return (0);
	str++;
	if (!parse_component(&str, &b))
		return (0);
	if (*str == ',')
		return (-1);
	if (*str != '\0' && *str != '\n')
		return (0);
	if (r > 255 || g > 255 || b > 255)
		return (0);
	*color = ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
	return (1);
}
