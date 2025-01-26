/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:19:45 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/26 03:22:30 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "MLX42/MLX42.h"
# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>

# define TILE_SIZE 32
# define MIN_TILE_SIZE 10

typedef struct s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef struct s_cell
{
	char			key;
	int				is_pacman;
	int				is_pill;
	int				is_mega_pill;
	int				is_wall;
	int				is_ghost;
	mlx_image_t		*image;
}					t_cell;

typedef struct s_ghost
{
	char			key_in_map;
	t_pos			pos;
	int				is_scared;
	int				is_dead;
	char			status_cell;
	char			direction;
	mlx_texture_t	**ghost;
	mlx_texture_t	**scared;
	mlx_texture_t	**dead;
}					t_ghost;

typedef struct s_pacman
{
	char			key_in_map;
	t_pos			pos;
	int				lives;
	int				is_dead;
	char			direction;
	int				is_power_up;
	mlx_texture_t	**alive;
	mlx_texture_t	**dead;
}					t_pacman;

typedef struct s_game
{
	mlx_t			*mlx;
	t_cell			**map;
	int				rows;
	int				cols;
	int				tile_size;
	int				x_offset;
	int				y_offset;
	mlx_texture_t	**map_textures;
	t_pacman		*pacman;
	t_ghost			**ghosts;
}					t_game;

// Declarations from game_utils.c
void				free_array_textures(mlx_texture_t **texture);
mlx_texture_t		**get_sprites(int fd, int limit);
t_pos				render_object(t_game *game, char key_in_map,
						mlx_texture_t *texture);
void				clear_images(t_game *game);
// Declarations from screen_utils.c
void				window_resize_handler(int32_t width, int32_t height,
						void *param);
void				update_tile_size(t_game *game);
// Declarations of pacman.c
void				free_pacman(t_pacman *pacman);
t_pacman			*create_pacman(t_game *game);

// Declarations from ghost.c
void				free_ghosts(t_ghost **ghost);
t_ghost				**create_ghosts(t_game *game, int num_ghosts);

// Declarations from map.c
void				get_map(t_game *game, char *path);
void				render_map(t_game *game);

#endif
