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

void	teleport_object(t_game *game, t_pacman *pacman, t_ghost *ghost)
{
	t_cell	cell;
	t_cell	cell2;

	ft_putstr_fd("Pills: ", STDOUT_FILENO);
	ft_putnbr_fd(game->pills, STDOUT_FILENO);
	cell = game->map[game->exit_pos[0].y][game->exit_pos[0].x];
	cell2 = game->map[game->exit_pos[0].y][game->exit_pos[1].x];
	if (pacman)
	{
		if (cell.is_pacman && !acces_cell(game, get_move(game->exit_pos[0],
					game->pacman->dir), 0))
			game->pacman->pos = game->exit_pos[1];
		else if (cell2.is_pacman && !acces_cell(game,
				get_move(game->exit_pos[1], game->pacman->dir), 0))
			game->pacman->pos = game->exit_pos[0];
		render_pacman(game, game->pacman);
	}
	if (ghost)
	{
		if (cell.is_ghost)
			ghost->pos = game->exit_pos[1];
		else if (cell2.is_ghost)
			ghost->pos = game->exit_pos[0];
		render_ghost(game, ghost);
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

void	finish_game(t_game *game)
{
	t_cell	cell_pacman;

	cell_pacman = game->map[game->pacman->pos.y][game->pacman->pos.x];
	if (game->pacman->lives == 0)
	{
		game->running = false;
		ft_putendl_fd("You lose!", STDOUT_FILENO);
		mlx_close_window(game->mlx);
	}
	if (game->pills == 0 && cell_pacman.is_exit)
	{
		ft_putendl_fd("You win!", STDOUT_FILENO);
		game->running = false;
		mlx_close_window(game->mlx);
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
