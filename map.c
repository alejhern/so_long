/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:04:16 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/25 04:07:57 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
/*
void	free_map(t_cell **map)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
	{

	}
}
*/
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
			map[pos.y][pos.x].key = map_str[pos.y][pos.x];
			map[pos.y][pos.x].is_pacman = (map_str[pos.y][pos.x] == 'P');
			map[pos.y][pos.x].is_pill = (map_str[pos.y][pos.x] == 'C');
			map[pos.y][pos.x].is_mega_pill = (map_str[pos.y][pos.x] == 'M');
			map[pos.y][pos.x].is_wall = (map_str[pos.y][pos.x] == '1');
			map[pos.y][pos.x].is_ghost = (map_str[pos.y][pos.x] == 'G');
			map[pos.y][pos.x].image = NULL;
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
	if (map_str)
		return (game->rows = ft_memlen(map_str),
			game->cols = ft_strlen(map_str[0]), game->map = get_map_cell(game,
				map_str), ft_free_array((void ***)&map_str));
	ft_free_array((void ***)&map_str);
}
