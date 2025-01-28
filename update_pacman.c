/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pacman.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:25:57 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/27 12:26:00 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	handle_mega_pill(t_game *game, t_cell *cell_pacman)
{
	int	index;

	cell_pacman->is_mega_pill = 0;
	game->pacman->state = POWER_UP;
	game->pacman->power_up_delay = game->pacman_timer + 500;
	game->score += 50;
	index = -1;
	while (game->ghosts[++index])
	{
		if (game->ghosts[index]->state != DEAD)
			game->ghosts[index]->state = SCARED;
	}
}

static void	manage_powe_up(t_game *game)
{
	int	index;

	if (game->pacman->state == POWER_UP)
	{
		if (game->pacman_timer >= game->pacman->power_up_delay)
		{
			game->pacman->state = ACTIVE;
			game->pacman->animation_delay = game->pacman_timer + 50;
			index = -1;
			while (game->ghosts[++index])
				if (game->ghosts[index]->state != DEAD
					&& game->ghosts[index]->state != WAITING)
					game->ghosts[index]->state = ACTIVE;
		}
	}
	else
		game->pacman->power_up_delay = game->pacman_timer + 50;
}

void	update_pacman_state(void *param)
{
	t_game	*game;
	t_cell	*cell_pacman;

	game = (t_game *)param;
	if (game->pacman->state == WAITING)
		return ;
	game->pacman_timer++;
	if (game->pacman->state == DEAD)
		kill_pacman(game);
	else if (game->pacman->state == REVIVED)
		revive_pacman(game);
	else
	{
		cell_pacman = &game->map[game->pacman->pos.y][game->pacman->pos.x];
		if (cell_pacman->is_pill)
		{
			cell_pacman->is_pill = 0;
			game->score += 10;
		}
		else if (cell_pacman->is_mega_pill)
			handle_mega_pill(game, cell_pacman);
		manage_powe_up(game);
	}
}
