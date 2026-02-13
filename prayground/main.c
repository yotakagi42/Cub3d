#include "cub3d.h"

int	game_loop(t_game *game)
{
	update_frame_time(game);
	process_movement(game);
	draw_floor_ceiling(game);
	raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int	main(void)
{
	t_game	game;

	init_game(&game);
	// KeyPress (event 2) と KeyRelease (event 3) を別々にフック
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 17, 1L << 17, close_window, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}
