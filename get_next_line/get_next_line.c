/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:24:43 by saciurus          #+#    #+#             */
/*   Updated: 2025/02/13 16:23:15 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*up_text(char *text, char *buffer)
{
	char	*tmp;

	if (!text)
	{
		tmp = ft_strdup(buffer);
		text = tmp;
	}
	else
	{
		tmp = ft_strjoin(text, buffer);
		free(text);
		text = tmp;
	}
	return (text);
}

char	*read_until_newline(int fd, char *text)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (text);
	while (!(text && ft_strchr(text, '\n')))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			free(text);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		text = up_text(text, buffer);
	}
	free(buffer);
	return (text);
}

char	*extract_line(char **text)
{
	char	*newline;
	char	*line;
	char	*tmp;
	int		len;

	newline = ft_strchr(*text, '\n');
	if (newline)
	{
		len = newline - *text + 1;
		line = ft_substr(*text, 0, len);
		tmp = ft_strdup(*text + len);
		free(*text);
		*text = tmp;
	}
	else
	{
		line = ft_strdup(*text);
		free(*text);
		*text = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*text = NULL;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	text = read_until_newline(fd, text);
	if (text && *text)
		return (extract_line(&text));
	free(text);
	text = NULL;
	return (NULL);
}
// #include <fcntl.h>
// #include <stdio.h>

// int	main()
// {
// 	int	fd = open("blabla.txt", O_RDONLY);
// 	char	*line;

// 	if (fd == -1)
// 	{
// 		printf("Error !");
// 		return (1);
// 	}
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }