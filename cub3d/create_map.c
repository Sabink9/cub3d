#include "cub3d.h"

static void	strip_newline(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

static int	ft_strncmp(const char *s1, const char *s2, int n)
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

static int	parse_color(char *str, unsigned int *color)
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

static char	*trim_path(char *str)
{
	int		len;
	char	*path;
	int		i;

	while (*str == ' ' || *str == '\t')
		str++;
	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' '
			|| str[len - 1] == '\t' || str[len - 1] == '\r'))
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

static int	parse_header_line(t_mlx *data, char *line, int *flags)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (data->tex[NORTH].path = trim_path(line + 3), *flags |= 1, 1);
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (data->tex[SOUTH].path = trim_path(line + 3), *flags |= 2, 1);
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (data->tex[EAST].path = trim_path(line + 3), *flags |= 4, 1);
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (data->tex[WEST].path = trim_path(line + 3), *flags |= 8, 1);
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_color(line + 2, &data->floor_color), *flags |= 16, 1);
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_color(line + 2, &data->ceil_color), *flags |= 32, 1);
	return (0);
}

static int	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line[i] || line[i] == '\n')
		return (0);
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' '
			&& line[i] != 'N' && line[i] != 'S' && line[i] != 'E'
			&& line[i] != 'W' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

static int	has_all_headers(char *line, int *flags)
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

static int	count_map_lines(int fd, int *flags)
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

static void	pad_row(char **row, int target_width)
{
	int		len;
	char	*padded;
	int		i;

	len = ft_strlen(*row);
	if (len >= target_width)
		return ;
	padded = malloc(target_width + 1);
	if (!padded)
		return ;
	i = 0;
	while (i < len)
	{
		padded[i] = (*row)[i];
		i++;
	}
	while (i < target_width)
		padded[i++] = ' ';
	padded[i] = '\0';
	free(*row);
	*row = padded;
}

static void	fill_grid(t_map *map, int fd)
{
	char	*line;
	int		y;
	int		in_map;
	int		flags;

	flags = 0;
	in_map = 0;
	y = 0;
	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		exit(1);
	line = get_next_line(fd);
	while (line)
	{
		strip_newline(line);
		if (!in_map)
			has_all_headers(line, &flags);
		if (flags == 63 && is_map_line(line))
			in_map = 1;
		if (in_map && line[0] != '\0')
		{
			map->grid[y] = ft_strdup(line);
			if (ft_strlen(map->grid[y]) > map->width)
				map->width = ft_strlen(map->grid[y]);
			y++;
		}
		free(line);
		line = get_next_line(fd);
	}
	map->grid[y] = NULL;
	y = 0;
	while (map->grid[y])
	{
		pad_row(&map->grid[y], map->width);
		y++;
	}
}

static void	find_player(t_map *map, t_player *player)
{
	int	row;
	int	col;

	row = 0;
	while (row < map->height)
	{
		col = 0;
		while (col < map->width)
		{
			if (map->grid[row][col] == 'N' || map->grid[row][col] == 'S'
				|| map->grid[row][col] == 'E' || map->grid[row][col] == 'W')
			{
				player->player_x = col + 0.5;
				player->player_y = row + 0.5;
				if (map->grid[row][col] == 'N')
					player->dir_angle = 270.0;
				else if (map->grid[row][col] == 'S')
					player->dir_angle = 90.0;
				else if (map->grid[row][col] == 'E')
					player->dir_angle = 0.0;
				else
					player->dir_angle = 180.0;
				map->grid[row][col] = '0';
				return ;
			}
			col++;
		}
		row++;
	}
}

static void	parse_headers(t_mlx *data, int fd, int *flags)
{
	char	*line;

	line = get_next_line(fd);
	while (line && *flags != 63)
	{
		strip_newline(line);
		parse_header_line(data, line, flags);
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
}

int	parse_map(t_mlx *data, char **av)
{
	int	fd;
	int	flags;
	int	height_flags;

	data->map.grid = NULL;
	data->map.width = 0;
	data->map.height = 0;
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		return (printf("Error: cannot open file\n"), 0);
	height_flags = 0;
	data->map.height = count_map_lines(fd, &height_flags);
	close(fd);
	if (data->map.height == 0)
		return (printf("Error: no map found\n"), 0);
	flags = 0;
	fd = open(av[1], O_RDONLY);
	parse_headers(data, fd, &flags);
	close(fd);
	if (flags != 63)
		return (printf("Error: missing identifiers in .cub file\n"), 0);
	fd = open(av[1], O_RDONLY);
	fill_grid(&data->map, fd);
	close(fd);
	find_player(&data->map, &data->player);
	return (1);
}
