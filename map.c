/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:04:16 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/25 23:52:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	render_map(t_game *game)
{
	t_pos	pos;

	update_tile_size(game);
	pos.y = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
		{
			if (game->map[pos.y][pos.x].is_wall)
				game->map[pos.y][pos.x].image = mlx_texture_to_image(game->mlx,
						game->map_textures[0]);
			else if (game->map[pos.y][pos.x].is_pill)
				game->map[pos.y][pos.x].image = mlx_texture_to_image(game->mlx,
						game->map_textures[1]);
			else if (game->map[pos.y][pos.x].is_mega_pill)
				game->map[pos.y][pos.x].image = mlx_texture_to_image(game->mlx,
						game->map_textures[2]);
			if (game->map[pos.y][pos.x].image)
				mlx_image_to_window(game->mlx, game->map[pos.y][pos.x].image,
					game->x_offset + pos.x * game->tile_size, game->y_offset
					+ pos.y * game->tile_size);
		}
	}
}

static void	new_cell_constructor(t_cell *cell, t_pos pos, char **map_str)
{
	cell->key = map_str[pos.y][pos.x];
	cell->is_pacman = (map_str[pos.y][pos.x] == 'P');
	cell->is_pill = (map_str[pos.y][pos.x] == 'C');
	cell->is_mega_pill = (map_str[pos.y][pos.x] == 'M');
	cell->is_wall = (map_str[pos.y][pos.x] == '1');
	cell->is_ghost = (map_str[pos.y][pos.x] == 'G');
	cell->is_exit = (map_str[pos.y][pos.x] == 'E');
	cell->image = NULL;
}

static void	update_essentials(t_game *game, t_cell cell, t_pos pos)
{
	if (cell.is_exit && game->exit_pos[0].x == -1)
		game->exit_pos[0] = pos;
	else if (cell.is_exit && game->exit_pos[1].x == -1)
		game->exit_pos[1] = pos;
	if (cell.is_pill || cell.is_mega_pill)
		game->pills++;
	if (cell.is_ghost)
		game->ghosts_count++;
}

static t_cell	**get_map_cell(t_game *game, char **map_str)
{
	t_pos	pos;
	t_cell	**map;

	map = ft_calloc((game->rows + 1), sizeof(t_cell *));
	if (!map)
		return (NULL);
	pos.y = -1;
	while (++pos.y < game->rows)
	{
		map[pos.y] = ft_calloc((game->cols + 1), sizeof(t_cell));
		if (!map[pos.y])
			return (ft_free_array((void ***)&map), NULL);
		pos.x = -1;
		while (++pos.x < game->cols)
		{
			new_cell_constructor(&map[pos.y][pos.x], pos, map_str);
			update_essentials(game, map[pos.y][pos.x], pos);
		}
	}
	return (map);
}

void	get_map(t_game *game, char *path)
{
	int		fd;
	char	*buffer;
	char	*content;
	char	**map_str;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return ;
	content = NULL;
	while (true)
	{
		buffer = get_next_line(fd);
		if (!buffer)
			break ;
		content = ft_strappend(content, buffer);
		free(buffer);
	}
	close(fd);
	map_str = ft_split(content, '\n');
	free(content);
	if (map_str && map_checker(map_str, game))
		return (game->map = get_map_cell(game, map_str),
			ft_free_array((void ***)&map_str));
	ft_free_array((void ***)&map_str);
}
