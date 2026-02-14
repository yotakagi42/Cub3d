#include "cub3d.h"

//毎回フレームレートによって移動量と回転量を計算する
//keyが押されているならprocess_movementで移動させる
//draw_floor_ceilingで床と天井を描画する
//raycastingで壁を描画する
//mlx_put_image_to_windowで画像を表示する
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

	//mapのパースをしてほしい
	//壁の判定とmap内の空白を０に置き換える
	//playerの初期位置を設定する(詳細はinit_player)
	//parse用の関数内でinit_mapとinit_playerを呼び出しちゃって
	//init_game内は呼び出し消すとかでいいかも
	init_game(&game);
	// KeyPress (event 2) と KeyRelease (event 3) を別々にフック
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 17, 1L << 17, close_window, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	return (0);
}
