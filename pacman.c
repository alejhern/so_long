/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pacman.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 20:07:13 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/24 20:07:18 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_pacman(t_pacman *pacman)
{
	int	index;

	if (pacman->alive)
	{
		index = -1;
		while (pacman->alive[++index])
			mlx_delete_texture(pacman->alive[index]);
		free(pacman->alive);
	}
	if (pacman->dead)
	{
		index = -1;
		while (pacman->dead[++index])
			mlx_delete_texture(pacman->dead[index]);
		free(pacman->dead);
	}
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
	pacman->speed = 1;
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
