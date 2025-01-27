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

static void	handle_ghost_collision(t_game *game, t_pacman *pacman, t_cell *cell)
{
	t_ghost	*ghost;

	ghost = game->ghosts[cell->is_ghost - 1];
	if (pacman->state != POWER_UP && pacman->state != REVIVED)
	{
		pacman->state = DEAD;
		cell->is_pacman = 0;
		game->score -= 120;
	}
	else if (pacman->state == POWER_UP)
	{
		game->score += 200;
		ghost->state = DEAD;
		cell->is_ghost = 0;
	}
	game->running = false;
}

static void	manage_powe_up(t_game *game)
{
	if (game->pacman->state == POWER_UP)
	{
		if (game->pacman_timer >= game->pacman->power_up_delay)
		{
			game->pacman->state = ACTIVE;
			game->pacman->power_up_delay = game->pacman_timer + 25;
		}
	}
	else
		game->pacman->power_up_delay = game->pacman_timer + 50;
}

static void	handle_death_animation(t_game *game)
{
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->animation_sprites = ft_memlen(game->pacman->dead);
		game->pacman->animation_delay = game->pacman_timer + 10;
	}
	if (game->pacman_timer >= game->pacman->animation_delay)
	{
		ft_rotate_array((void ***)&game->pacman->dead);
		render_pacman(game, game->pacman);
		game->pacman->animation_sprites--;
		game->pacman->animation_delay = game->pacman_timer + 10;
	}
	if (game->pacman->animation_sprites == 0)
	{
		mlx_delete_image(game->mlx, game->pacman->image);
		game->pacman->pos = game->pacman->init_pos;
		game->pacman->state = REVIVED;
		game->pacman->animation_sprites = 0;
		game->pacman->animation_delay = 0;
	}
}

static void	handle_revive_animation(t_game *game)
{
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->animation_sprites = ft_memlen(game->pacman->dead);
		game->pacman->animation_delay = game->pacman_timer + 10;
	}
	if (game->pacman_timer >= game->pacman->animation_delay)
	{
		game->pacman->state = DEAD;
		ft_rotate_rev_array((void ***)&game->pacman->dead);
		render_pacman(game, game->pacman);
		game->pacman->state = REVIVED;
		game->pacman->animation_sprites--;
		game->pacman->animation_delay = game->pacman_timer + 10;
	}
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->state = WAITING;
		game->pacman->animation_sprites = 0;
		game->pacman->pos = game->pacman->init_pos;
		render_pacman(game, game->pacman);
	}
}

void	update_pacman_state(void *param)
{
	t_game	*game;
	t_cell	*cell_pacman;

	game = (t_game *)param;
	game->pacman_timer++;
	if (game->pacman->state == DEAD)
		handle_death_animation(game);
	else if (game->pacman->state == REVIVED)
		handle_revive_animation(game);
	else
	{
		cell_pacman = &game->map[game->pacman->pos.y][game->pacman->pos.x];
		if (cell_pacman->is_pill)
			cell_pacman->is_pill = 0;
		else if (cell_pacman->is_mega_pill)
		{
			cell_pacman->is_mega_pill = 0;
			game->pacman->state = POWER_UP;
			game->pacman->power_up_delay = game->pacman_timer + 500;
			game->score += 50;
		}
		else if (cell_pacman->is_ghost)
			handle_ghost_collision(game, game->pacman, cell_pacman);
		manage_powe_up(game);
	}
}
