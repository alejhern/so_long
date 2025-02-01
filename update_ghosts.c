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

int	get_probabilityes(t_game *game, t_ghost *ghost)
{
	int	dir;
	int	count;

	dir = -1;
	count = 0;
	while (++dir < 4)
		if (acces_cell(game, get_move(ghost->pos, (t_dir)dir), 0))
			count++;
	return (count);
}

static t_dir	opposite_dir(t_dir dir)
{
	if (dir == UP)
		return (DOWN);
	if (dir == DOWN)
		return (UP);
	if (dir == LEFT)
		return (RIGHT);
	if (dir == RIGHT)
		return (LEFT);
	return (dir);
}

static void	update_dir(t_game *game, t_ghost *ghost)
{
	t_dir	possibilities[4];
	int		access[2];
	t_pos	new_pos;
	double	distance;
	double	best_distance;

	access[0] = -1;
	access[1] = 0;
	while (++access[0] < 4)
		if (acces_cell(game, get_move(ghost->pos, (t_dir)access[0]), ghost->id)
			&& (t_dir)access[0] != opposite_dir(ghost->dir))
			possibilities[access[1]++] = (t_dir)access[0];
	best_distance = game->cols * game->rows;
	while (--access[1] >= 0)
	{
		new_pos = get_move(ghost->pos, possibilities[access[1]]);
		distance = sqrt(pow(new_pos.x - ghost->target.x, 2) + pow(new_pos.y
					- ghost->target.y, 2));
		if (distance < best_distance && !ft_pos_cmp(new_pos, ghost->prev_pos))
		{
			best_distance = distance;
			ghost->dir = possibilities[access[1]];
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
		else if (ghost->state == ACTIVE)
			ghost->target = game->pacman->pos;
		else if (ghost->state == DEAD && ft_pos_cmp(ghost->pos,
				ghost->init_pos))
			ghost->state = REVIVED;
		else if (ghost->state == DEAD)
			ghost->target = ghost->init_pos;
		update_dir(game, ghost);
		if (game->map[ghost->pos.y][ghost->pos.x].is_exit)
			teleport_object(game, NULL, ghost);
	}
}
