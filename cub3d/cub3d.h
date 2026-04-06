/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 15:59:33 by saciurus          #+#    #+#             */
/*   Updated: 2026/04/06 16:16:54 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define EAST 2
# define WEST 3

typedef struct s_tex
{
	void			*img;
	char			*addr;
	int				w;
	int				h;
	int				bpp;
	int				line_len;
	int				endian;
	char			*path;
}					t_tex;

typedef struct s_player
{
	double			player_x;
	double			player_y;
	double			dir_angle;
}					t_player;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
}					t_map;

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
}					t_mlx;

typedef struct s_ray
{
	double			distance;
	double			hit_x;
	double			hit_y;
	int				side;
}					t_ray;

typedef struct s_parse
{
	int				y;
	int				in_map;
	int				flags;
}					t_parse;

typedef struct s_square
{
	int				x;
	int				y;
	int				size;
	int				color;
}					t_square;

typedef struct s_hand
{
	int				tex_x;
	int				tex_y;
	int				screen_x;
	int				screen_y;
	char			*pixel_src;
	unsigned int	color;
	char			*pixel_dst;
	int				dx;
	int				dy;
	int				*info;
}					t_hand;

typedef struct s_collision
{
	double			margin;
	int				x_min;
	int				x_max;
	int				y_min;
	int				y_max;
}					t_collision;

typedef struct s_dda
{
	double			side_dist[2];
	double			delta_dist[2];
	double			rdx;
	double			rdy;
	int				step[2];
	int				map[2];
}					t_dda;

typedef struct s_column
{
	int				wall_height;
	int				draw_start;
	int				draw_end;
	int				side;
	int				tex_x;
}					t_column;

/* Parsing */
// Utils
void				strip_newline(char *line);
int					ft_strncmp(const char *s1, const char *s2, int n);
int					parse_color(char *str, unsigned int *color);
// Headers
int					parse_header_line(t_mlx *data, char *line, int *flags);
int					is_map_line(char *line);
int					has_all_headers(char *line, int *flags);
int					count_map_lines(int fd, int *flags);
// Grid
void				fill_grid(t_map *map, int fd);
// Map
int					parse_map(t_mlx *data, char **av);

/* Raycasting */
// dda
void				dda_step(t_dda *d, int *side);
void				init_dda(t_mlx *data, double angle, t_dda *d);
// Wall
t_ray				out_of_bounds(void);
t_ray				hit_wall(t_mlx *data, t_dda *d, int side);
// Render
void				render_3d(t_mlx *data);

/* Rendering */
// Minimap
void				draw_minimap(t_mlx *data);
// Hands
void				draw_hands(t_mlx *data);
// Keys
int					close_window(t_mlx *data);
int					key_press(int keycode, t_mlx *data);
int					key_release(int keycode, t_mlx *data);
void				init_keys(t_mlx *data);
// Moovs
void				handle_movement(t_mlx *data, double *new_x, double *new_y,
						int *moved);
void				handle_strafe(t_mlx *data, double *new_x, double *new_y,
						int *moved);
void				handle_rotation(t_mlx *data, int *moved);
int					check_row(t_mlx *data, int y, int x_min, int x_max);
int					check_collision(t_mlx *data, double new_x, double new_y);

#endif
