/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:27 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/27 11:11:11 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	acces_cell(t_game *game, t_ghost *ghost, t_pos pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= game->cols || pos.y >= game->rows)
		return (0);
	if (ghost)
		if (ft_pos_cmp(pos, ghost->prev_pos))
			return (0);
	return (!game->map[pos.y][pos.x].is_wall);
}

static t_pos	best_move(t_game *game, t_ghost *gh, t_pos ini, t_pos obj)
{
	t_pos	move;

	move = (t_pos){0, 0};
	if (obj.y > ini.y && acces_cell(game, gh, (t_pos){ini.x, ini.y + 1}))
		move.y = 1;
	else if (obj.y < ini.y && acces_cell(game, gh, (t_pos){ini.x, ini.y - 1}))
		move.y = -1;
	else if (obj.x > ini.x && acces_cell(game, gh, (t_pos){ini.x + 1, ini.y}))
		move.x = 1;
	else if (obj.x < ini.x && acces_cell(game, gh, (t_pos){ini.x - 1, ini.y}))
		move.x = -1;
	if (move.x == 0 && move.y == 0)
	{
		if (acces_cell(game, gh, (t_pos){ini.x + 1, ini.y}))
			move.x = 1;
		else if (acces_cell(game, gh, (t_pos){ini.x, ini.y + 1}))
			move.y = 1;
		else if (acces_cell(game, gh, (t_pos){ini.x, ini.y - 1}))
			move.y = -1;
		else if (acces_cell(game, gh, (t_pos){ini.x - 1, ini.y}))
			move.x = -1;
	}
	return (move);
}

static void	move_ghost(t_game *game, t_ghost *ghost, int gh_id)
{
	t_pos	old_pos;
	t_pos	new_pos;
	int		draw_x;
	int		draw_y;

	old_pos = ghost->pos;
	new_pos = best_move(game, ghost, old_pos, game->pacman->pos);
	new_pos = ft_pos_add(old_pos, new_pos);
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

void	move_ghosts(void *param)
{
	t_game	*game;
	int		index;
	t_ghost	*ghost;

	game = (t_game *)param;
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
			ghost->delay = game->timer + 50;
		}
	}
}

void	move_pacman(t_game *game, t_pos pos)
{
	t_pos	new_pos;
	t_pos	old_pos;
	int		draw_x;
	int		draw_y;

	old_pos = game->pacman->pos;
	new_pos = ft_pos_add(old_pos, pos);
	if (acces_cell(game, NULL, new_pos))
	{
		render_pacman(game, game->pacman);
		mlx_set_instance_depth(&game->pacman->image->instances[0], -1);
		game->map[old_pos.y][old_pos.x].is_pacman = 0;
		if (game->map[new_pos.y][new_pos.x].is_pill
			|| game->map[new_pos.y][new_pos.x].is_mega_pill)
			mlx_delete_image(game->mlx, game->map[new_pos.y][new_pos.x].image);
		game->map[new_pos.y][new_pos.x].is_pacman = 1;
		draw_x = game->x_offset + new_pos.x * game->tile_size;
		draw_y = game->y_offset + new_pos.y * game->tile_size;
		game->pacman->image->instances[0].x = draw_x;
		game->pacman->image->instances[0].y = draw_y;
		mlx_set_instance_depth(&game->pacman->image->instances[0], 0);
		game->pacman->pos = new_pos;
		ft_printf("MOVE COUNT --> %d\n", ++game->count_move);
	}
}
