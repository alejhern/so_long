/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:53:16 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/27 11:33:49 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->running)
		move_ghosts(game, game->timer++);
}

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (keydata.key == MLX_KEY_W)
		move_pacman(game, (t_pos){0, -1});
	else if (keydata.key == MLX_KEY_A)
		move_pacman(game, (t_pos){-1, 0});
	else if (keydata.key == MLX_KEY_S)
		move_pacman(game, (t_pos){0, 1});
	else if (keydata.key == MLX_KEY_D)
		move_pacman(game, (t_pos){1, 0});
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	game->running = true;
}
