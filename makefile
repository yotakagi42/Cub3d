NAME		= cub3D
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRC_DIR		= src/Parse
GNL_DIR		= includes/Get_next_line
LIBFT_DIR	= includes/libft

SRCS		= $(SRC_DIR)/main.c \
			  $(GNL_DIR)/get_next_line.c \
			  $(GNL_DIR)/get_next_line_utils.c

OBJS		= $(SRCS:.c=.o)

INCLUDES	= -I $(SRC_DIR) -I $(GNL_DIR) -I $(LIBFT_DIR)

LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
		@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
