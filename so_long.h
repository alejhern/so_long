/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:19:45 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/26 14:02:27 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "MLX42/MLX42.h"
# include "libft/libft.h"
# include <math.h>

# define TILE_SIZE 32
# define MIN_TILE_SIZE 10

# define MAP_TEXTURES "routes/pacman-base.txt"
# define PACMAN_ALIVE "routes/pacman-alive.txt"
# define PACMAN_DEAD "routes/pacman-dead.txt"
# define PACMAN_SCARED "routes/pacman-scared.txt"
# define GHOSTS "routes/ghosts.txt"
# define GHOST_DEAD "routes/ghost-dead.txt"
# define GHOST_SCARED "routes/ghost-scared.txt"

# define PACMAN_DELAY 5
# define PACMAN_POWER_UP_DELAY 4
# define PACMAN_POWER_UP_TIME_OUT 500
# define PACMAN_ANIMATION_DELAY 10
# define GHOST_DELAY 10
# define GHOST_WAITING_DELAY 300

typedef struct s_cell
{
	char			key;
	int				is_pacman;
	int				is_pill;
	int				is_mega_pill;
	int				is_wall;
	int				is_ghost;
	int				is_exit;
	mlx_image_t		*image;
}					t_cell;

typedef enum e_state
{
	WAITING,
	ACTIVE,
	SCARED,
	DEAD,
	REVIVED,
	POWER_UP
}					t_states;

typedef enum e_dir
{
	LEFT,
	RIGHT,
	UP,
	DOWN
}					t_dir;

typedef struct s_ghost
{
	char			key_in_map;
	int				id;
	t_pos			pos;
	t_pos			init_pos;
	t_pos			prev_pos;
	t_pos			target;
	int				delay;
	t_dir			dir;
	t_states		state;
	mlx_texture_t	**ghost;
	mlx_texture_t	**scared;
	mlx_texture_t	**dead;
	mlx_image_t		*image;
}					t_ghost;

typedef struct s_pacman
{
	char			key_in_map;
	t_pos			pos;
	t_pos			init_pos;
	int				lives;
	int				power_up_delay;
	int				delay;
	int				animation_sprites;
	t_dir			dir;
	t_states		state;
	mlx_texture_t	**alive;
	mlx_texture_t	**dead;
	mlx_image_t		*image;
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
	int				running;
	int				count_move;
	int				ghosts_count;
	int				pills;
	int				timer;
	int				pacman_timer;
	t_pos			exit_pos[2];
	int				score;
	int				frame;
	mlx_texture_t	**map_textures;
	t_pacman		*pacman;
	t_ghost			**ghosts;
}					t_game;

// Declarations from game_events.c
void				teleport_object(t_game *game, t_pacman *pacman,
						t_ghost *ghost);
void				ghost_pacman_collision(t_game *game);
void				ghost_ghost_collision(t_game *game);
void				finish_game(t_game *game);
void				key_handler(mlx_key_data_t keydata, void *param);

// Declarations of img_move.c
int					acces_cell(t_game *game, t_pos pos, int gh_id);
void				move_ghosts(t_game *game);
void				move_pacman(t_game *game);

// Declarations from update_pacman.c
void				update_pacman_state(t_game *game);

// Declarations of pacman.c
void				render_pacman(t_game *game, t_pacman *pacman);
void				free_pacman(mlx_t *mlx, t_pacman *pacman);
t_pacman			*create_pacman(t_game *game);

// Declarations from update_ghosts.c
int					get_probabilityes(t_game *game, t_ghost *ghost);
void				update_ghosts_state(t_game *game);

// Declarations from ghost.c
void				free_ghosts(mlx_t *mlx, t_ghost **ghost);
void				render_ghost(t_game *game, t_ghost *ghost);
t_ghost				**create_ghosts(t_game *game);

// Declarations from screen_utils.c
void				update_tile_size(t_game *game);
void				window_resize_handler(int32_t width, int32_t height,
						void *param);

// Declarations from map_checker.c
int					map_checker(char **map_str, t_game *game);

// Declarations from map.c
void				render_map(t_game *game);
void				get_map(t_game *game, char *path);

// Declarations from game_utils.c
t_pos				get_move(t_pos pos, t_dir dir);
void				clear_images(t_game *game);
mlx_image_t			*regenerate_sprite(t_game *game, mlx_texture_t *texture,
						t_pos pos);
mlx_texture_t		**get_sprites(int fd, int limit);
t_pos				get_init_pos(t_game *game, char key_in_map, int ignore);

#endif
