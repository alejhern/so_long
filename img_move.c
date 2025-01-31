/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:27 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/28 18:00:17 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	acces_cell(t_game *game, t_pos pos, int gh_id)
{
	t_pos	prev_pos;

	if (pos.x < 0 || pos.y < 0 || pos.x >= game->cols || pos.y >= game->rows)
		return (0);
	if (gh_id--)
	{
		prev_pos = game->ghosts[gh_id]->pos;
		if ((game->map[prev_pos.y][prev_pos.x].key == 'G'
			&& game->map[pos.y][pos.x].key == 'G') || get_probabilityes(game,
				game->ghosts[gh_id]) == 1)
			return (1);
		else if (game->map[pos.y][pos.x].key == 'G'
			&& game->ghosts[gh_id]->state != DEAD)
			return (0);
	}
	return (!game->map[pos.y][pos.x].is_wall);
}

static void	move_ghost(t_game *game, t_ghost *ghost, int gh_id)
{
	t_pos	old_pos;
	t_pos	new_pos;
	int		draw_x;
	int		draw_y;

	old_pos = ghost->pos;
	update_ghosts_state(game);
	new_pos = get_move(ghost->pos, ghost->dir);
	if (!acces_cell(game, new_pos, gh_id))
		return ;
	mlx_set_instance_depth(&ghost->image->instances[0], -1);
	game->map[old_pos.y][old_pos.x].is_ghost = 0;
	if (ghost->state != DEAD)
		game->map[new_pos.y][new_pos.x].is_ghost = gh_id;
	else
		game->map[new_pos.y][new_pos.x].is_ghost = 0;
	draw_x = game->x_offset + new_pos.x * game->tile_size;
	draw_y = game->y_offset + new_pos.y * game->tile_size;
	ghost->image->instances[0].x = draw_x;
	ghost->image->instances[0].y = draw_y;
	mlx_set_instance_depth(&ghost->image->instances[0], 0);
	ghost->pos = new_pos;
	ghost->prev_pos = old_pos;
}

void	move_ghosts(t_game *game)
{
	int		index;
	t_ghost	*ghost;

	if (!game->running)
		return ;
	game->timer++;
	index = -1;
	while (game->ghosts[++index])
	{
		ghost = game->ghosts[index];
		if (ghost->state == WAITING && game->timer >= ghost->delay)
			ghost->state = ACTIVE;
		else if (game->timer < ghost->delay)
			continue ;
		if (ghost->state != WAITING)
		{
			render_ghost(game, ghost);
			move_ghost(game, ghost, index + 1);
			ghost->delay = game->timer + GHOST_DELAY;
		}
	}
}

void	move_pacman(t_game *game)
{
	t_pos	new_pos;

	if (!game->running)
		return ;
	new_pos = get_move(game->pacman->pos, game->pacman->dir);
	if (game->timer >= game->pacman->delay && acces_cell(game, new_pos, 0))
	{
		render_pacman(game, game->pacman);
		mlx_set_instance_depth(&game->pacman->image->instances[0], -1);
		game->map[game->pacman->pos.y][game->pacman->pos.x].is_pacman = 0;
		if (game->map[new_pos.y][new_pos.x].is_pill
			|| game->map[new_pos.y][new_pos.x].is_mega_pill)
			mlx_delete_image(game->mlx, game->map[new_pos.y][new_pos.x].image);
		game->map[new_pos.y][new_pos.x].is_pacman = 1;
		game->pacman->image->instances[0].x = game->x_offset + new_pos.x
			* game->tile_size;
		game->pacman->image->instances[0].y = game->y_offset + new_pos.y
			* game->tile_size;
		mlx_set_instance_depth(&game->pacman->image->instances[0], 0);
		game->pacman->pos = new_pos;
		ft_printf("MOVE COUNT --> %d\n", ++game->count_move);
		game->pacman->delay = game->timer + PACMAN_DELAY;
		if (game->pacman->state == POWER_UP)
			game->pacman->delay = game->timer + PACMAN_POWER_UP_DELAY;
	}
}
