/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:33:11 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/17 21:33:17 by amhernandez      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_array_texrures(mlx_texture_t **texture)
{
	int	index;

	index = -1;
	while (texture[++index])
	{
		mlx_delete_texture(texture[index]);
		free(texture[index]);
	}
	free(texture);
}

mlx_texture_t	**get_sprites(int fd, int limit)
{
	mlx_texture_t	**sprites;
	int				index;
	char			*path;

	sprites = malloc(sizeof(mlx_texture_t *) * limit);
	if (!sprites)
		return (NULL);
	index = -1;
	while (true && ++index < limit)
	{
		path = get_next_line(fd);
		if (!path)
			break ;
		if (ft_strchr(path, '\n'))
			ft_strchr(path, '\n')[0] = '\0';
		sprites[index] = mlx_load_png(path);
		free(path);
		if (!sprites[index])
			return (free_array_texrures(sprites), NULL);
	}
	return (sprites);
}

void	clear_images(t_game *game)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
		{
			if (game->map[pos.y][pos.x].image)
			{
				mlx_delete_image(game->mlx, game->map[pos.y][pos.x].image);
				game->map[pos.y][pos.x].image = NULL;
			}
		}
	}
}

static void	regenerate_sprites(t_game *game, mlx_texture_t *texture, t_pos pos)
{
	int			draw_x;
	int			draw_y;
	mlx_image_t	*img;

	draw_x = game->x_offset + pos.x * game->tile_size;
	draw_y = game->y_offset + pos.y * game->tile_size;
	img = mlx_texture_to_image(game->mlx, texture);
	if (game->map[pos.x][pos.y].image)
		mlx_delete_image(game->mlx, game->map[pos.y][pos.x].image);
	game->map[pos.y][pos.x].image = img;
	mlx_image_to_window(game->mlx, img, draw_x, draw_y);
}

t_pos	render_object(t_game *game, char key_in_map, mlx_texture_t *texture)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
		{
			if (game->map[pos.y][pos.x].key == key_in_map)
			{
				regenerate_sprites(game, texture, pos);
				return (pos);
			}
		}
	}
	return (pos);
}
