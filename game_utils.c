/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amhernandez <alejhern@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:33:11 by amhernandez       #+#    #+#             */
/*   Updated: 2025/01/26 06:58:19 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

t_pos	get_direction_offset(t_dir dir)
{
	t_pos	offset;

	offset = (t_pos){0, 0};
	if (dir == LEFT)
		offset.x = -1;
	else if (dir == RIGHT)
		offset.x = 1;
	else if (dir == UP)
		offset.y = -1;
	else if (dir == DOWN)
		offset.y = 1;
	return (offset);
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

mlx_image_t	*regenerate_sprite(t_game *game, mlx_texture_t *texture, t_pos pos)
{
	mlx_image_t	*img;
	int			draw_x;
	int			draw_y;

	img = mlx_texture_to_image(game->mlx, texture);
	if (!img)
		return (NULL);
	draw_x = game->x_offset + pos.x * game->tile_size;
	draw_y = game->y_offset + pos.y * game->tile_size;
	mlx_image_to_window(game->mlx, img, draw_x, draw_y);
	return (img);
}

mlx_texture_t	**get_sprites(int fd, int limit)
{
	mlx_texture_t	**sprites;
	int				index;
	char			*path;

	sprites = ft_calloc((limit + 1), sizeof(mlx_texture_t *));
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
			return (ft_free_func_array((void ***)&sprites,
					(void (*)(void *))mlx_delete_texture), NULL);
	}
	return (sprites);
}

t_pos	get_init_pos(t_game *game, char key_in_map, int ignore)
{
	t_pos	pos;
	int		ignored;

	pos.y = -1;
	ignored = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
		{
			if (game->map[pos.y][pos.x].key == key_in_map
				&& ++ignored == ignore)
				return (pos);
		}
	}
	return (pos);
}
