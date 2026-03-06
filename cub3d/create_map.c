#include "cub3d.h"
#include <stdio.h>

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
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (first_line_width);
}

void	fill_grid(t_map *map, int fd)
{
	char	*line;
	int		y;

	y = 0;
	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		exit(1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		map->grid[y] = line;
		y++;
		line = get_next_line(fd);
	}
        map->grid[y] = NULL;
}

void	print_map(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->height)
	{
		write(1, map->grid[i], ft_strlen(map->grid[i]));
                //write(1, "\n", 1);
		i++;
	}
}

void    find_player(t_map *map, t_player *player)
{
    int row;
    int col;

    row = 0;
    while (row < map->height)
    {
        col = 0;
        while (col < map->width)
        {
            if (map->grid[row][col] == 'N')
            {
                player->player_x = col;
                player->player_y = row;
                return ;
            }
            col++;
        }
        row++;
    }
}

int parse_map(t_mlx *data, char **av)
{
    int     fd;

    data->map.grid = NULL;
    data->map.width = 0;
    data->map.height = 0;
    fd = open(av[1], O_RDONLY);
    if (fd == -1)
        return (printf("pas bon"), 0);
    data->map.height = count_lines(fd);
    close(fd);
    fd = open(av[1], O_RDONLY);
    data->map.width = count_rows(fd);
    close(fd);
    fd = open(av[1], O_RDONLY);
    fill_grid(&data->map, fd);
    close(fd);
    find_player(&data->map, &data->player);
    return (0);
}