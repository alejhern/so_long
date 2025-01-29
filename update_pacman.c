/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pacman.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:25:57 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/28 19:45:18 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	handle_mega_pill(t_game *game, t_cell *cell_pacman)
{
	int	index;

	cell_pacman->is_mega_pill = 0;
	game->pacman->state = POWER_UP;
	game->pacman->power_up_delay = game->timer + PACMAN_POWER_UP_TIME_OUT;
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
		if (game->timer >= game->pacman->power_up_delay)
		{
			game->pacman->state = ACTIVE;
			index = -1;
			while (game->ghosts[++index])
				if (game->ghosts[index]->state != DEAD
					&& game->ghosts[index]->state != WAITING)
					game->ghosts[index]->state = ACTIVE;
		}
	}
}

void	update_ghosts_state(t_game *game)
{
	int		index;
	t_ghost	*ghost;

	if (!game->running)
		return ;
	index = -1;
	while (game->ghosts[++index])
	{
		ghost = game->ghosts[index];
		if (ghost->state == WAITING)
			continue ;
		else if (ghost->state == REVIVED)
			ghost->state = ACTIVE;
		else if (ghost->state == DEAD && ft_pos_cmp(ghost->pos,
				ghost->init_pos))
			ghost->state = REVIVED;
	}
}

void	update_pacman_state(t_game *game)
{
	t_cell	*cell_pacman;

	if (game->pacman->state == WAITING)
		return ;
	game->pacman_timer++;
	if (game->pacman->state == DEAD)
		kill_pacman(game);
	else if (game->pacman->state == REVIVED)
		revive_pacman(game);
	else
	{
		if (!game->running)
			return ;
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
