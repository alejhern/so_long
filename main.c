/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 07:05:18 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/26 13:55:17 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	new_game_constructor(t_game *game)
{
	game->rows = 0;
	game->cols = 0;
	game->tile_size = TILE_SIZE;
	game->x_offset = 0;
	game->y_offset = 0;
	game->running = false;
	game->count_move = 0;
	game->ghosts_count = 0;
	game->pills = 0;
	game->timer = 0;
	game->pacman_timer = 0;
	game->score = 0;
	game->frame = 0;
	game->exit_pos[0].x = -1;
	game->exit_pos[1].x = -1;
	game->map_textures = NULL;
	game->map = NULL;
	game->pacman = NULL;
	game->ghosts = NULL;
}

void	update_move_counter(t_game *game)
{
	char	*nb;
	char	*str;

	nb = ft_itoa(game->count_move);
	str = ft_strjoin("Movimientos: ", nb);
	mlx_put_string(game->mlx, str, 30, 10);
	free(str);
	free(nb);
}

static void	game_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_pacman_state(game);
	finish_game(game);
	ghost_ghost_collision(game);
	update_ghosts_state(game);
	move_ghosts(game);
	move_pacman(game);
	ghost_pacman_collision(game);
	update_move_counter(game);
	update_tile_size(game);
}

static void	load_game(t_game *game)
{
	int	fd;

	new_game_constructor(game);
	get_map(game, "sprites/map/map.ber");
	if (!game->map)
		return ;
	fd = open(MAP_TEXTURES, O_RDONLY);
	if (fd == -1)
		return ;
	game->map_textures = get_sprites(fd, 3);
	close(fd);
	if (!game->map_textures)
		return ;
	render_map(game);
	game->ghosts = create_ghosts(game);
	if (!game->ghosts)
		return ;
	game->pacman = create_pacman(game);
	if (!game->pacman)
		return ;
	mlx_key_hook(game->mlx, key_handler, game);
	mlx_loop_hook(game->mlx, game_update, game);
	mlx_resize_hook(game->mlx, window_resize_handler, game);
	mlx_loop(game->mlx);
}

int	main(void)
{
	t_game	game;

	game.mlx = mlx_init(1600, 1200, "Pac-Man", true);
	if (!game.mlx)
		return (EXIT_FAILURE);
	load_game(&game);
	free_ghosts(game.mlx, game.ghosts);
	free_pacman(game.mlx, game.pacman);
	ft_free_func_array((void ***)&game.map_textures,
		(void (*)(void *))mlx_delete_texture);
	if (game.map)
		clear_images(&game);
	ft_free_array((void ***)&game.map);
	mlx_terminate(game.mlx);
	return (EXIT_SUCCESS);
}
