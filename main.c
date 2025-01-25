
#include "so_long.h"

// void key_handler(mlx_key_data_t keydata, void* param) {
//     t_game* game = (t_game*)param;

//     if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
//         mlx_close_window(game->mlx);

//     if (keydata.action == MLX_PRESS) {
//         int new_x = game->player_x;
//         int new_y = game->player_y;

//         if (keydata.key == MLX_KEY_W) new_y--;
//         else if (keydata.key == MLX_KEY_S) new_y++;
//         else if (keydata.key == MLX_KEY_A) new_x--;
//         else if (keydata.key == MLX_KEY_D) new_x++;

//         if (game->map[new_y][new_x] != '1') {
//             game->player_x = new_x;
//             game->player_y = new_y;
//         }
//     }
// }

void	init_game(t_game *game)
{
	int			fd;
	t_ghost		**ghosts;
	t_pacman	*pacman;

	get_map(game, "sprites/map/map.ber");
	fd = open("routes/pacman-base.txt", O_RDONLY);
	if (fd == -1)
		return ;
	game->map_textures = get_sprites(fd, 3);
	close(fd);
	render_map(game);
	ghosts = create_ghosts(game, 4);
	if (!ghosts)
		return ;
	pacman = create_pacman(game);
	if (!ghosts)
		return (free_ghosts(ghosts));
	mlx_loop(game->mlx);
	free_ghosts(ghosts);
	free_pacman(pacman);
	free_array_textures(game->map_textures);
}

int	main(void)
{
	t_game	game;

	// t_pacman pacman;
	// t_ghost ghost;
	game.mlx = mlx_init(800, 600, "Pac-Man", true);
	if (!game.mlx)
		return (EXIT_FAILURE);
	init_game(&game);
	// pacman = create_pacman(&game);
	// ghost = create_ghost(&game, 4);
	// update_tile_size(&game);
	// render_map(&game);
	// mlx_key_hook(game.mlx, key_handler, &game);
	mlx_resize_hook(game.mlx, window_resize_handler, &game);
	// Liberar recursos
	clear_images(&game);
	ft_free_array((void ***)&game.map);
	mlx_terminate(game.mlx);
	return (EXIT_SUCCESS);
}
