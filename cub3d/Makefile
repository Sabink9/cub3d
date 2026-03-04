CC = cc
CFLAGS = -Wall -Werror -Wextra -I.././get_next_line
LDFLAGS = -L .././minilibx-linux -lmlx -lX11 -lXext
RM = rm -rf

SRC =	render_map.c \
	create_map.c \
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

re: fclean all

.PHONY: all clean fclean re