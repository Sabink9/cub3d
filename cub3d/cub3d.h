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

typedef struct s_player
{
	int			size;
	int			color;
	int			speed;
	double player_x; // CHANGE int en double
	double player_y; // CHANGE int en double
	double prev_x;   // CHANGE int en double
	double prev_y;   // CHANGE int en double
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
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			tile_size;

	void		*wall_texture;
	char		*wall_addr;
	int			wall_w;
	int			wall_h;
	int			wall_bpp;
	int			wall_line_len;
	int			wall_endian;

	void		*hands_texture;
	char		*hands_addr;
	int			hands_w;
	int			hands_h;
	int			hands_bpp;
	int			hands_line_len;
	int			hands_endian;

	t_player	player;
	t_map		map;
	int			keys[65536];
}				t_mlx;

typedef struct s_ray
{
	double		distance;
	double		hit_x;
	double		hit_y;
	int side;
}				t_ray;

// Parsing
int				parse_map(t_mlx *data, char **av);

// Raycasting
t_ray			cast_ray(t_mlx *data, double angle);
void			render_3d(t_mlx *data);

// Rendering
void			draw_minimap(t_mlx *data);
void			draw_hands(t_mlx *data);
void			draw_square(t_mlx *data, int x, int y, int size, int color);

// Events
int				key_press(int keycode, t_mlx *data);
int				key_release(int keycode, t_mlx *data);
int				game_loop(t_mlx *data);
int				close_window(t_mlx *data);

#endif
