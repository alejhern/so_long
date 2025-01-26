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

t_pos	best_move(t_game *game, t_pos ini, t_pos obj)
{
	t_pos	move;

	move = (t_pos){0, 0};
	if (obj.y > ini.y && acces_cell(game, (t_pos){ini.x, ini.y + 1}))
		move.y = 1;
	else if (obj.y < ini.y && acces_cell(game, (t_pos){ini.x, ini.y - 1}))
		move.y = -1;
	else if (obj.x > ini.x && acces_cell(game, (t_pos){ini.x + 1, ini.y}))
		move.x = 1;
	else if (obj.x < ini.x && acces_cell(game, (t_pos){ini.x - 1, ini.y}))
		move.x = -1;
	if (move.x == 0 && move.y == 0)
	{
		if (acces_cell(game, (t_pos){ini.x + 1, ini.y}))
			move.x = 1;
		else if (acces_cell(game, (t_pos){ini.x, ini.y + 1}))
			move.y = 1;
		else if (acces_cell(game, (t_pos){ini.x, ini.y - 1}))
			move.y = -1;
		else if (acces_cell(game, (t_pos){ini.x - 1, ini.y}))
			move.x = -1;
	}
	return (move);
}

void	move_ghost(t_game *game, t_ghost *ghost)
{
	t_pos	old_pos;
	t_pos	new_pos;
	int		draw_x;
	int		draw_y;

	old_pos = ghost->pos;
	new_pos = best_move(game, old_pos, game->pacman->pos);
	new_pos = ft_pos_add(old_pos, new_pos);
	mlx_set_instance_depth(&ghost->image->instances[0], -1);
	game->map[old_pos.y][old_pos.x].is_ghost = 0;
	game->map[new_pos.y][new_pos.x].is_ghost = 1;
	draw_x = game->x_offset + new_pos.x * game->tile_size;
	draw_y = game->y_offset + new_pos.y * game->tile_size;
	ghost->image->instances[0].x = draw_x;
	ghost->image->instances[0].y = draw_y;
	mlx_set_instance_depth(&ghost->image->instances[0], 0);
	ghost->pos = new_pos;
}

void	move_ghosts(t_game *game, int current_time)
{
	int		index;
	t_ghost	*ghost;

	index = -1;
	while (game->ghosts[++index])
	{
		ghost = game->ghosts[index];
		if (ghost->state == WAITING && current_time >= ghost->delay)
			ghost->state = ACTIVE;
		if (ghost->state == ACTIVE)
		{
			move_ghost(game, ghost);
			ghost->delay = current_time + 50;
			ghost->state = WAITING;
		}
	}
}

void	game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->running)
		move_ghosts(game, game->timer++);
}
