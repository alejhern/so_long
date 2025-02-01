/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:17:49 by amhernandez       #+#    #+#             */
/*   Updated: 2025/02/01 20:35:01 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	update_tile_size(t_game *game)
{
	int	window_width;
	int	window_height;
	int	tile_width;
	int	tile_height;

	window_width = game->mlx->width;
	window_height = game->mlx->height;
	tile_width = window_width / game->cols;
	tile_height = window_height / game->rows;
	if (tile_width < tile_height)
		game->tile_size = tile_width;
	else
		game->tile_size = tile_height;
	game->x_offset = (window_width - (game->cols * game->tile_size)) / 2;
	game->y_offset = (window_height - (game->rows * game->tile_size)) / 2;
}

void	window_resize_handler(int32_t width, int32_t height, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	game->mlx->width = width;
	game->mlx->height = height;
	update_tile_size(game);
}
