# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 19:56:30 by amhernandez       #+#    #+#              #
#    Updated: 2025/01/14 19:56:40 by amhernandez      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

NAME = so_long

SRCS = main.c
OBJ = $(SRCS:.c=.o)

LIB_DIR = libft/
LIBFT = $(LIB_DIR)libft.a

MLX42_DIR = MLX42/
MLX42_LIB = $(MLX42_DIR)build/libmlx42.a
MLX42_INC = $(MLX42_DIR)/include

CC = cc
CFLAGS = -Wall -Werror -Wextra -I $(LIB_DIR) -I $(MLX42_INC)

LDFLAGS = -L$(LIB_DIR) -lft -L$(MLX42_DIR)/build -lmlx42 -ldl -lglfw -lm -lpthread

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(LIBFT) $(MLX42_LIB) $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX42_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c
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

clean:
	@make -C $(LIB_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@make -C $(LIB_DIR) fclean
	rm -f $(NAME)
	rm -rf $(MLX42_DIR)build

re: fclean all

.PHONY: all clean fclean re
