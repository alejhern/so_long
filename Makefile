# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 19:56:30 by amhernandez       #+#    #+#              #
#    Updated: 2025/01/26 07:56:09 by alejhern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

NAME = so_long

SRCS =  screen_utils.c			\
		game_utils.c			\
		update_pacman.c			\
		update_ghosts.c			\
		game_events.c			\
		img_move.c				\
		ghost.c					\
		pacman.c				\
		map.c					\
		main.c

OBJ = $(SRCS:.c=.o)

LIB_DIR = libft/
LIBFT = $(LIB_DIR)libft.a

MLX42_DIR = MLX42/
MLX42_LIB = $(MLX42_DIR)build/libmlx42.a
MLX42_INC = $(MLX42_DIR)/include

CC = cc -g
CFLAGS = -Wall -Werror -Wextra -I $(LIB_DIR) -I $(MLX42_INC)

LDFLAGS = -L$(LIB_DIR) -lft -L$(MLX42_DIR)/build -lmlx42 -ldl -lglfw -lm -lpthread

ID_FILE = "1fUfL2GhSGRLVCnpXsm_pUwFXT1rwhXTW"
RESOURCES_WWW = "https://drive.google.com/uc?export=download&id=$(ID_FILE)"

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(LIBFT) $(MLX42_LIB) $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX42_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c so_long.h	Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@if [ ! -d "$(LIB_DIR)" ]; then \
		git clone https://github.com/alejhern/libft.git $(LIB_DIR); \
	fi
	@make -C $(LIB_DIR)

$(MLX42_LIB):
	@if [ ! -d "$(MLX42_DIR)" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX42_DIR); \
	fi
	@cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/build
	@make -C $(MLX42_DIR)/build

download_resources:
	@if [ ! -d "sprites" ] || [ ! -d "routes" ]; then \
		echo "Downloading the sprites and routes directories..."; \
		wget $(RESOURCES_WWW) -O so_long_resources.tar > /dev/null 2>&1; \
		tar -xpvf so_long_resources.tar > /dev/null; \
		rm -f so_long_resources.tar; \
	fi

clean:
	@make -C $(LIB_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@make -C $(LIB_DIR) fclean
	rm -f $(NAME)
	rm -rf $(MLX42_DIR)build

re: fclean all

.PHONY: all clean fclean re
