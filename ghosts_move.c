/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ghosts_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:50:45 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/26 14:23:59 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <time.h>

// Generar un movimiento aleatorio
t_pos	random_move(void)
{
	t_pos	moves[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

	return (moves[rand() % 4]); // Elegir aleatoriamente una dirección
}

// Mover a un fantasma
void	move_ghost(t_game *game, t_ghost *ghost)
{
	t_pos	move;
	int		new_x;
	int		new_y;

	move = random_move();
	new_x = ghost->pos.x + move.x;
	new_y = ghost->pos.y + move.y;
	if (is_valid_move(game, new_x, new_y))
	{
		// Actualizar mapa y posición lógica
		game->map[ghost->pos.y][ghost->pos.x].is_ghost = 0;
		ghost->pos.x = new_x;
		ghost->pos.y = new_y;
		game->map[new_y][new_x].is_ghost = 1;
		// Redibujar al fantasma
		mlx_image_to_window(game->mlx, ghost->ghost[0], new_x * game->tile_size
			+ game->x_offset, new_y * game->tile_size + game->y_offset);
	}
}

void move_ghosts(t_game *game, int num_ghosts, int current_time)
{
	int	index;

	index = -1;
    while (game-ghosts[++index])
    {
		if (ghost->state == WAITING && current_time >= ghost->delay)
			game->ghost[index].state = ACTIVE;
        if (ghosts[index]->state == ACTIVE)
            move_ghost(game, game->ghosts[index], current_time);
    }
}

void	game_loop(t_game *game)
{
	int current_time;

	current_time = 0;
	while (game->running)
	{
		///handle_pacman(game);
		move_ghosts(game, game->ghosts, game->num_ghosts, current_time);
		current_time += 5;
	}
}
