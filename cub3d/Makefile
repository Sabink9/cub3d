CC = cc
CFLAGS = -Wall -Werror -Wextra -I.././get_next_line
LDFLAGS = -L .././minilibx-linux -lmlx -lX11 -lXext -lm
ASAN_FLAGS = -fsanitize=address,leak -g
RM = rm -rf

SRC =	render_game/draw_hands.c \
	render_game/draw_minimap.c \
	render_game/key_and_window.c \
	render_game/moovs_and_env.c \
	render_game/main.c \
	parse_map/create_map.c \
	parse_map/fill_grid.c \
	parse_map/headers_and_lines.c \
	parse_map/utile_parse_map.c \
	parse_map/validate_map.c \
	parse_map/player.c \
	parse_map/parse_color.c \
	raycasting/dda.c \
	raycasting/hit_wall.c \
	raycasting/render_cast.c \
	../get_next_line/get_next_line.c \
	../get_next_line/get_next_line_utils.c 

OBJS = $(SRC:.c=.o)
NAME = cub3d

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

asan: fclean
	$(MAKE) all CFLAGS="$(CFLAGS) $(ASAN_FLAGS)" LDFLAGS="$(LDFLAGS) $(ASAN_FLAGS)"

re: fclean all

.PHONY: all clean fclean re asan