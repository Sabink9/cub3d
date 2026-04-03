#ifndef CUB3D_H
# define CUB3D_H

# include "../get_next_line/get_next_line.h"
# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIN_W 800
# define WIN_H 600
# define FOV 60.0

/* Wall face directions */
# define NORTH 0
# define SOUTH 1
# define EAST  2
# define WEST  3

typedef struct s_tex
{
	void	*img;
	char	*addr;
	int		w;
	int		h;
	int		bpp;
	int		line_len;
	int		endian;
	char	*path;
}				t_tex;

typedef struct s_player
{
	double		player_x;
	double		player_y;
	double		dir_angle;
}				t_player;

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
}				t_map;

typedef struct s_mlx
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	t_tex			tex[4];
	void			*hands_texture;
	char			*hands_addr;
	int				hands_w;
	int				hands_h;
	int				hands_bpp;
	int				hands_line_len;
	int				hands_endian;
	unsigned int	floor_color;
	unsigned int	ceil_color;
	t_player		player;
	t_map			map;
	int				keys[65536];
}				t_mlx;

typedef struct s_ray
{
	double	distance;
	double	hit_x;
	double	hit_y;
	int		side;
}				t_ray;

/* Parsing */
int		parse_map(t_mlx *data, char **av);

/* Raycasting */
t_ray	cast_ray(t_mlx *data, double angle);
void	render_3d(t_mlx *data);

/* Rendering */
void	draw_minimap(t_mlx *data);
void	draw_hands(t_mlx *data);
void	draw_square(t_mlx *data, int x, int y, int size, int color);

/* Events */
int		key_press(int keycode, t_mlx *data);
int		key_release(int keycode, t_mlx *data);
int		game_loop(t_mlx *data);
int		close_window(t_mlx *data);

#endif
