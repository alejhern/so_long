/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 05:49:40 by alejhern          #+#    #+#             */
/*   Updated: 2025/02/01 07:09:24 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	check_keys(char **map_str, t_game *game)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
			if (map_str[pos.y][pos.x] != 'P' && map_str[pos.y][pos.x] != 'C'
				&& map_str[pos.y][pos.x] != 'M' && map_str[pos.y][pos.x] != '1'
				&& map_str[pos.y][pos.x] != 'G' && map_str[pos.y][pos.x] != 'E'
				&& map_str[pos.y][pos.x] != '0')
				return (0);
	}
	return (1);
}

static int	check_closed(char **map_str, t_game *game)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
		if (map_str[pos.y][0] != 'E' && map_str[pos.y][0] != '1')
			return (0);
	pos.x = -1;
	while (++pos.x < game->cols)
		if (map_str[0][pos.x] != 'E' && map_str[0][pos.x] != '1')
			return (0);
	pos.y = game->rows;
	while (--pos.y >= 0)
		if (map_str[pos.y][game->cols - 1] != 'E' && map_str[pos.y][game->cols
			- 1] != '1')
			return (0);
	pos.x = game->cols;
	while (--pos.x >= 0)
		if (map_str[game->rows - 1][pos.x] != 'E' && map_str[game->rows
			- 1][pos.x] != '1')
			return (0);
	return (1);
}

static int	check_pills(char **map_str, t_game *game)
{
	t_pos	pos;

	pos.y = -1;
	while (++pos.y < game->rows)
	{
		pos.x = -1;
		while (++pos.x < game->cols)
			if (map_str[pos.y][pos.x] == 'C')
				game->pills++;
	}
	if (game->pills > 0)
		return (1);
	return (0);
}

int	map_checker(char **map_str, t_game *game)
{
	game->rows = ft_memlen(map_str);
	game->cols = ft_strlen(map_str[0]);
	if (!check_keys(map_str, game))
	{
		ft_putstr_fd("The map has invalid keys\n", STDERR_FILENO);
		return (0);
	}
	if (!check_closed(map_str, game))
	{
		ft_putstr_fd("The map is not closed\n", STDERR_FILENO);
		return (0);
	}
	if (!check_pills(map_str, game))
	{
		ft_putstr_fd("The map has no pills\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}
