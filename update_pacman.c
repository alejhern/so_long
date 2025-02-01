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

static void	kill_pacman(t_game *game)
{
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->animation_sprites = ft_memlen(game->pacman->dead);
		game->pacman->delay = game->pacman_timer + 10;
	}
	if (game->pacman_timer >= game->pacman->delay)
	{
		ft_rotate_array((void ***)&game->pacman->dead);
		render_pacman(game, game->pacman);
		game->pacman->animation_sprites--;
		game->pacman->delay = game->pacman_timer + 10;
	}
	if (game->pacman->animation_sprites == 0)
	{
		mlx_delete_image(game->mlx, game->pacman->image);
		game->pacman->pos = game->pacman->init_pos;
		game->pacman->state = REVIVED;
		game->pacman->animation_sprites = 0;
		game->pacman->delay = 0;
	}
}

static void	revive_pacman(t_game *game)
{
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->animation_sprites = ft_memlen(game->pacman->dead);
		game->pacman->delay = game->pacman_timer + 10;
	}
	if (game->pacman_timer >= game->pacman->delay)
	{
		game->pacman->state = DEAD;
		ft_rotate_rev_array((void ***)&game->pacman->dead);
		render_pacman(game, game->pacman);
		game->pacman->state = REVIVED;
		game->pacman->animation_sprites--;
		game->pacman->delay = game->pacman_timer + 10;
	}
	if (game->pacman->animation_sprites == 0)
	{
		game->pacman->state = WAITING;
		game->pacman->animation_sprites = 0;
		game->pacman->lives--;
		game->pacman->delay = game->timer;
		game->pacman->pos = game->pacman->init_pos;
		render_pacman(game, game->pacman);
	}
}

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
		if (cell_pacman->is_exit)
			teleport_object(game, game->pacman, NULL);
	}
}
