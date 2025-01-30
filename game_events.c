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

void	revive_pacman(t_game *game)
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
		game->pacman->delay = game->timer;
		game->pacman->pos = game->pacman->init_pos;
		render_pacman(game, game->pacman);
	}
}

void	ghost_ghost_collision(t_game *game)
{
	t_ghost	**gh_iters[2];

	gh_iters[0] = game->ghosts;
	while (*gh_iters[0])
	{
		if ((*gh_iters[0])->state == DEAD || (*gh_iters[0])->state == WAITING)
		{
			gh_iters[0]++;
			continue ;
		}
		gh_iters[1] = gh_iters[0];
		while (*++gh_iters[1])
		{
			if ((*gh_iters[1])->state == DEAD
				|| (*gh_iters[1])->state == WAITING)
				continue ;
			if (ft_pos_distance((*gh_iters[0])->pos, (*gh_iters[1])->pos) == 1
				&& (((*gh_iters[0])->dir + (*gh_iters[1])->dir) % 2))
			{
				(*gh_iters[0])->dir = ((*gh_iters[0])->dir + 2) % 4;
				(*gh_iters[1])->dir = ((*gh_iters[1])->dir + 2) % 4;
			}
		}
		gh_iters[0]++;
	}
}

void	ghost_pacman_collision(t_game *game)
{
	int	index;

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
	t_dir	dir;

	game = (t_game *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (game->pacman->state == DEAD || game->pacman->state == REVIVED)
		return ;
	else if (game->pacman->state == WAITING)
		game->pacman->state = ACTIVE;
	dir = game->pacman->dir;
	if (keydata.key == MLX_KEY_W)
		game->pacman->dir = UP;
	else if (keydata.key == MLX_KEY_A)
		game->pacman->dir = LEFT;
	else if (keydata.key == MLX_KEY_S)
		game->pacman->dir = DOWN;
	else if (keydata.key == MLX_KEY_D)
		game->pacman->dir = RIGHT;
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	if (!acces_cell(game, get_move(game->pacman->pos, game->pacman->dir), 0))
		game->pacman->dir = dir;
	game->running = true;
}
