/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pacman.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:13 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/26 07:03:23 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	new_pacman_constructor(t_pacman *pacman, t_game *game)
{
	pacman->key_in_map = 'P';
	pacman->init_pos = get_init_pos(game, pacman->key_in_map, 0);
	pacman->pos = pacman->init_pos;
	pacman->lives = 3;
	pacman->direction = 'R';
	pacman->is_power_up = 0;
	pacman->is_dead = false;
	pacman->alive = NULL;
	pacman->dead = NULL;
	pacman->image = NULL;
}

void	free_pacman(mlx_t *mlx, t_pacman *pacman)
{
	if (!pacman)
		return ;
	free_array_textures(pacman->alive);
	free_array_textures(pacman->dead);
	if (pacman->image)
		mlx_delete_image(mlx, pacman->image);
	free(pacman);
	pacman = NULL;
}

void	render_pacman(t_game *game, t_pacman *pacman)
{
	mlx_texture_t	*texture;

	texture = pacman->alive[0];
	if (pacman->image)
		mlx_delete_image(game->mlx, pacman->image);
	pacman->image = regenerate_sprite(game, texture, pacman->pos);
	if (!pacman->image)
		free_pacman(game->mlx, pacman);
}

t_pacman	*create_pacman(t_game *game)
{
	t_pacman	*pacman;
	int			fd;

	pacman = malloc(sizeof(t_pacman));
	if (!pacman)
		return (NULL);
	new_pacman_constructor(pacman, game);
	fd = open("routes/pacman-alive.txt", O_RDONLY);
	if (fd == -1)
		return (free_pacman(game->mlx, pacman), NULL);
	pacman->alive = get_sprites(fd, 3);
	close(fd);
	fd = open("routes/pacman-dead.txt", O_RDONLY);
	if (fd == -1)
		return (free_pacman(game->mlx, pacman), NULL);
	pacman->dead = get_sprites(fd, 10);
	close(fd);
	if (!pacman->alive || !pacman->dead)
		return (free_pacman(game->mlx, pacman), NULL);
	render_pacman(game, pacman);
	return (pacman);
}
