#include "cub3d.h"


int	count_lines(int fd)
{
	int		height;
	char	*line;

	height = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		height++;
		free(line);
		line = get_next_line(fd);
	}
	return (height);
}
void	free_remaining_lines(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		line = get_next_line(fd);
	}
}
int	count_rows(int fd)
{
	int		first_line_width;
	int		current_width;
	char	*line;

	first_line_width = -1;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		current_width = ft_strlen(line);
		if (first_line_width == -1)
			first_line_width = current_width;
		else if (current_width != first_line_width)
		{
			free(line);
			free_remaining_lines(fd);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (first_line_width);
}

int	main(int argc, char **argv)
{
	int	fd;
	int	width;

	if (argc != 2)
	{
		printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (1);
	}
	width = count_rows(fd);
	if (width == -1)
		printf("Erreur : la map n'est pas rectangulaire !\n");
	else
		printf("Largeur de la map : %d\n", width);
	close(fd);
	return (0);
}
