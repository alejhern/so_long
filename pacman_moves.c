/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pacman_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:00:27 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/26 13:53:31 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	acces_cell(t_game *game, t_pos pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x >= game->cols || pos.y >= game->rows)
		return (0);
	return (!game->map[pos.y][pos.x].is_wall);
}

void	move_pacman(t_game *game, int dx, int dy)
{
	t_pos	new_pos;
	t_pos	old_pos;
	int		draw_x;
	int		draw_y;

	old_pos = game->pacman->pos;
	new_pos.x = old_pos.x + dx;
	new_pos.y = old_pos.y + dy;
	if (acces_cell(game, new_pos))
	{
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

void	key_handler(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (keydata.key == MLX_KEY_W)
		move_pacman(game, 0, -1);
	else if (keydata.key == MLX_KEY_A)
		move_pacman(game, -1, 0);
	else if (keydata.key == MLX_KEY_S)
		move_pacman(game, 0, 1);
	else if (keydata.key == MLX_KEY_D)
		move_pacman(game, 1, 0);
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	game->running = true;
}
