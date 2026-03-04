#ifndef CUB3D_H
# define CUB3D_H

# include "../get_next_line/get_next_line.h"
# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
}				t_map;

typedef struct s_player
{
	int			size;
	int			color;
	int			speed;
	int			player_x;
	int			player_y;
}				t_player;

typedef struct s_mlx
{
	void		*mlx;
	void		*win;
	int			tile_size;
	t_player	player;
	t_map		map;
}				t_mlx;

int parse_map(t_map *map, char **av);

#endif
