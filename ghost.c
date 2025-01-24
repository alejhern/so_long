/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ghost.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:28:21 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/18 19:28:28 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	free_ghosts(t_ghost **ghosts)
{
	int	index_gh;
	int	index;

	index_gh = -1;
	while (ghosts[++index_gh])
	{
		index = -1;
		if (ghosts[index_gh]->ghost)
			while (ghosts[index_gh]->ghost[++index])
				mlx_delete_texture(ghosts[index_gh]->ghost[index]);
		index = -1;
		if (ghosts[index_gh]->dead)
			while (ghosts[index_gh]->dead[++index])
				mlx_delete_texture(ghosts[index_gh]->dead[index]);
		index = -1;
		if (ghosts[index_gh]->scared)
			while (ghosts[index_gh]->scared[++index])
				mlx_delete_texture(ghosts[index_gh]->scared[index]);
		free(ghosts[index_gh]);
	}
	free(ghosts);
	ghosts = NULL;
}

static void	get_ghost_sprites(t_ghost *ghost, int fd_gh)
{
	int	fd;

	ghost->ghost = get_sprites(fd_gh, 2);
	if (!ghost->ghost)
		return ;
	fd = open("routes/ghost-dead.txt", O_RDONLY);
	if (fd == -1)
		return ;
	ghost->dead = get_sprites(fd, 4);
	close(fd);
	if (!ghost->dead)
		return ;
	fd = open("routes/ghost-scared.txt", O_RDONLY);
	if (fd == -1)
		return ;
}

static void	new_ghost_constructor(t_ghost *ghost)
{
	ghost->key_in_map = 'G';
	ghost->is_dead = 0;
	ghost->is_scared = 0;
	ghost->status_cell = '0';
	ghost->speed = 1;
	ghost->ghost = NULL;
	ghost->dead = NULL;
	ghost->scared = NULL;
}

void	render_ghost(t_game *game, t_ghost *ghost)
{
	mlx_texture_t	*texture;

	texture = ghost->ghost[0];
	ghost->pos = render_object(game, ghost->key_in_map, texture);
}

t_ghost	**create_ghosts(t_game *game, int num_ghosts)
{
	int		fd;
	t_ghost	**ghosts;
	int		index;

	fd = open("routes/ghosts.txt", O_RDONLY);
	if (fd == -1)
		return (NULL);
	ghosts = malloc(sizeof(t_ghost *) * num_ghosts);
	index = -1;
	while (ghosts && ghosts[++index])
	{
		ghosts[index] = malloc(sizeof(t_ghost));
		if (!ghosts[index])
			return (free_ghosts(ghosts), close(fd), NULL);
		new_ghost_constructor(ghosts[index]);
		get_ghost_sprites(ghosts[index], fd);
		if (!ghosts[index]->ghost || !ghosts[index]->dead
			|| !ghosts[index]->scared)
			return (free_ghosts(ghosts), close(fd), NULL);
		render_ghost(game, ghosts[index]);
		if (!game->map[ghosts[index]->pos.y][ghosts[index]->pos.x].image)
			return (free_ghosts(ghosts), close(fd), NULL);
	}
	close(fd);
	return (ghosts);
}
