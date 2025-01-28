/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:53:16 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/27 11:33:49 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	kill_pacman(t_game *game)
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

void	revive_pacman(t_game *game)
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

void	ghost_ghost_collision(void *param)
{
	t_game	*game;
	int		index;
	int		index2;

	game = (t_game *)param;
	if (!game->running)
		return ;
	index = -1;
	while (game->ghosts[++index])
	{
		if (game->ghosts[index]->state == DEAD)
			continue ;
		index2 = index;
		while (game->ghosts[++index2])
		{
			if (game->ghosts[index2]->state == DEAD)
				continue ;
			if (ft_pos_distance(game->ghosts[index]->pos,
					game->ghosts[index2]->pos) == 1)
			{
				game->ghosts[index]->dir = (game->ghosts[index]->dir + 2) % 4;
				game->ghosts[index2]->dir = (game->ghosts[index2]->dir + 2) % 4;
			}
		}
	}
}

void	ghost_pacman_collision(void *param)
{
	t_game	*game;
	int		index;

	game = (t_game *)param;
	index = -1;
	while (game->ghosts[++index])
	{
		if (game->ghosts[index]->state == DEAD)
			continue ;
		if (ft_pos_cmp(game->pacman->pos, game->ghosts[index]->pos))
		{
			if (game->pacman->state == POWER_UP)
			{
				game->ghosts[index]->state = DEAD;
				game->score += 200;
			}
			else
			{
				game->score -= 150;
				game->pacman->state = DEAD;
				game->running = false;
				return ;
			}
		}
	}
}

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (game->pacman->state == DEAD)
		return ;
	else if (game->pacman->state == REVIVED)
		return ;
	else if (game->pacman->state == WAITING)
		game->pacman->state = ACTIVE;
	if (keydata.key == MLX_KEY_W)
		move_pacman(game, (t_pos){0, -1});
	else if (keydata.key == MLX_KEY_A)
		move_pacman(game, (t_pos){-1, 0});
	else if (keydata.key == MLX_KEY_S)
		move_pacman(game, (t_pos){0, 1});
	else if (keydata.key == MLX_KEY_D)
		move_pacman(game, (t_pos){1, 0});
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	game->running = true;
}
