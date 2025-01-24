/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:17:49 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/17 21:17:52 by amhernandez      ###   ########.fr       */
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
	// Calcula el tamaño del tile basado en las dimensiones de la ventana y el mapa
	tile_width = window_width / game->cols;
	tile_height = window_height / game->rows;
	// Escoge el menor tamaño para que el mapa completo quepa en la pantalla
	game->tile_size = (tile_width < tile_height) ? tile_width : tile_height;
	// Calcula los márgenes para centrar el mapa si hay espacio sobrante
	game->x_offset = (window_width - (game->cols * game->tile_size)) / 2;
	game->y_offset = (window_height - (game->rows * game->tile_size)) / 2;
}

void	window_resize_handler(int32_t width, int32_t height, void *param)
{
	t_game *game;

	game = (t_game *)param;
	game->mlx->width = width;
	game->mlx->height = height;
	update_tile_size(game);
}