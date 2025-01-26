/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pacman.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:13 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/26 03:50:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	free_pacman(t_pacman *pacman)
{
	if (!pacman)
		return ;
	free_array_textures(pacman->alive);
	free_array_textures(pacman->dead);
	free(pacman);
	pacman = NULL;
}

static void	render_pacman(t_game *game, t_pacman *pacman)
{
	mlx_texture_t	*texture;

	texture = pacman->alive[0];
	pacman->pos = render_object(game, pacman->key_in_map, texture);
	if (!game->map[pacman->pos.y][pacman->pos.x].image)
		free_pacman(pacman);
}

static void	new_pacman_constructor(t_pacman *pacman)
{
	pacman->key_in_map = 'P';
	pacman->lives = 3;
	pacman->direction = 'R';
	pacman->is_dead = false;
	pacman->alive = NULL;
	pacman->dead = NULL;
}

t_pacman	*create_pacman(t_game *game)
{
	t_pacman	*pacman;
	int			fd;

	pacman = malloc(sizeof(t_pacman));
	if (!pacman)
		return (NULL);
	new_pacman_constructor(pacman);
	fd = open("routes/pacman-alive.txt", O_RDONLY);
	if (fd == -1)
		return (free_pacman(pacman), NULL);
	pacman->alive = get_sprites(fd, 3);
	close(fd);
	fd = open("routes/pacman-dead.txt", O_RDONLY);
	if (fd == -1)
		return (free_pacman(pacman), NULL);
	pacman->dead = get_sprites(fd, 10);
	close(fd);
	if (!pacman->alive || !pacman->dead)
		return (free_pacman(pacman), NULL);
	render_pacman(game, pacman);
	return (pacman);
}
