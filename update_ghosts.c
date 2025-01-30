/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_ghosts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:53:59 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/29 13:54:03 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static t_dir	best_move(t_game *game, t_ghost *ghost)
{
	t_pos	pos_diff;
	t_dir	new_dir;
	t_dir	possibilities[4];
	int		access_count;
	int		dir;

	pos_diff = ft_pos_sub(ghost->target, ghost->pos);
	new_dir = ghost->dir;
	access_count = 0;
	dir = 0;
	while (dir < 4)
	{
		if (acces_cell(game, get_move(ghost->pos, (t_dir)dir), ghost->id))
			possibilities[access_count++] = (t_dir)dir;
		dir++;
	}
	if (access_count > 1)
	{
		if (ghost->dir == UP || ghost->dir == DOWN)
		{
			if (pos_diff.x > 0 && acces_cell(game, get_move(ghost->pos, RIGHT),
					ghost->id))
				return (RIGHT);
			if (pos_diff.x < 0 && acces_cell(game, get_move(ghost->pos, LEFT),
					ghost->id))
				return (LEFT);
		}
		else if (ghost->dir == LEFT || ghost->dir == RIGHT)
		{
			if (pos_diff.y > 0 && acces_cell(game, get_move(ghost->pos, DOWN),
					ghost->id))
				return (DOWN);
			if (pos_diff.y < 0 && acces_cell(game, get_move(ghost->pos, UP),
					ghost->id))
				return (UP);
		}
	}
	if (pos_diff.y == 0)
	{
		if (pos_diff.x > 0 && acces_cell(game, get_move(ghost->pos, RIGHT),
				ghost->id))
			return (RIGHT);
		if (pos_diff.x < 0 && acces_cell(game, get_move(ghost->pos, LEFT),
				ghost->id))
			return (LEFT);
	}
	if (pos_diff.x == 0)
	{
		if (pos_diff.y > 0 && acces_cell(game, get_move(ghost->pos, DOWN),
				ghost->id))
			return (DOWN);
		if (pos_diff.y < 0 && acces_cell(game, get_move(ghost->pos, UP),
				ghost->id))
			return (UP);
	}
	if (acces_cell(game, get_move(ghost->pos, new_dir), ghost->id))
		return (new_dir);
	return (possibilities[0]);
}

void	update_ghosts_state(t_game *game)
{
	int index;
	t_ghost *ghost;

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
		else if (ghost->state == ACTIVE)
			ghost->target = game->pacman->pos;
		else if (ghost->state == DEAD && ft_pos_cmp(ghost->pos,
				ghost->init_pos))
			ghost->state = REVIVED;
		else if (ghost->state == DEAD)
			ghost->target = ghost->init_pos;

		ghost->dir = best_move(game, ghost);
	}
}