NAME = cub3d
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx_Linux.a
INCLUDES = -I$(MLX_DIR) -I/usr/include
LIBS = -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz

SRC = main.c init.c input.c render.c raycast.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(MLX_LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

$(MLX_LIB):
	make -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)
	make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
