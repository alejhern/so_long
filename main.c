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

void	init_game(t_game *game)
{
	int	fd;

	get_map(game, "sprites/map/map.ber");
	fd = open("routes/pacman-base.txt", O_RDONLY);
	if (fd == -1)
		return ;
	game->map_textures = get_sprites(fd, 3);
	close(fd);
	if (!game->map_textures)
		return ;
	render_map(game);
	game->ghosts = create_ghosts(game, 4);
	if (!game->ghosts)
		return ;
	game->pacman = create_pacman(game);
	if (!game->pacman)
		return ;
	game->timer = 0;
	game->count_move = 0;
	game->running = false;
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_key_hook(game->mlx, key_handler, game);
	// mlx_resize_hook(game.mlx, window_resize_handler, &game);
	mlx_loop(game->mlx);
}

int	main(void)
{
	t_game	game;

	game.mlx = mlx_init(800, 600, "Pac-Man", true);
	if (!game.mlx)
		return (EXIT_FAILURE);
	init_game(&game);
	free_ghosts(game.mlx, game.ghosts);
	free_pacman(game.mlx, game.pacman);
	free_array_textures(game.map_textures);
	clear_images(&game);
	ft_free_array((void ***)&game.map);
	mlx_terminate(game.mlx);
	return (EXIT_SUCCESS);
}
