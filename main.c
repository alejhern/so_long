/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:16:33 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/11 00:52:03 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <MLX42/MLX42.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_SIZE 128
#define MOVE_SPEED 5

int					move_count = 0;

static mlx_image_t	*image;

// Crear un color en formato RGBA
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

// Llenar la imagen con colores aleatorios
void	ft_randomize(void *param)
{
	(void)param;
	for (uint32_t i = 0; i < image->width; ++i)
	{
		for (uint32_t y = 0; y < image->height; ++y)
		{
			uint32_t color = ft_pixel(rand() % 256, // R
										rand() % 256, // G
										rand() % 256, // B
										255           // A (opaco)
			);
			mlx_put_pixel(image, i, y, color);
		}
	}
}

// Controlar el movimiento de la imagen con las teclas
void	ft_hook(void *param)
{
	mlx_t	*mlx;
	int		max_x;
	int		max_y;
	int		last_count;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	// Obtener los límites dinámicos de la ventana
	max_x = mlx->width - IMAGE_SIZE;
	max_y = mlx->height - IMAGE_SIZE;
	last_count = move_count;
	// Mover la imagen dentro de los límites dinámicos
	if (mlx_is_key_down(mlx, MLX_KEY_UP) && image->instances[0].y > 0
		&& ++move_count)
		image->instances[0].y -= MOVE_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN) && image->instances[0].y < max_y
		&& ++move_count)
		image->instances[0].y += MOVE_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT) && image->instances[0].x > 0
		&& ++move_count)
		image->instances[0].x -= MOVE_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT) && image->instances[0].x < max_x
		&& ++move_count)
		image->instances[0].x += MOVE_SPEED;
	if (last_count < move_count)
		ft_printf_fd(STDOUT_FILENO, "Move count: %d\n", move_count);
}

int32_t	main(void)
{
	const int	width = 512;
	const int	height = 512;
	mlx_t		*mlx;

	// Configura el tamaño inicial de la ventana
	// Inicializar la ventana
	mlx = mlx_init(width, height, "MLX42", true);
	if (!mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	// Crear una imagen
	image = mlx_new_image(mlx, IMAGE_SIZE, IMAGE_SIZE);
	if (!image)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	// Enviar la imagen a la ventana
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	// Rellenar la imagen con colores aleatorios inicialmente
	ft_randomize(NULL);
	// Configurar hooks para actualizar la ventana y mover la imagen
	mlx_loop_hook(mlx, ft_hook, mlx);
	// Iniciar el bucle principal
	mlx_loop(mlx);
	// Terminar el programa
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
