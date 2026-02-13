#include "cub3d.h"

int	close_window(t_game *game)
{
	mlx_destroy_window(game->mlx, game->win);
	exit(0);
	return (0);
}

void	rotate_player(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->dir_x;
	game->dir_x = game->dir_x * cos(rot_speed) - game->dir_y * sin(rot_speed);
	game->dir_y = old_dir_x * sin(rot_speed) + game->dir_y * cos(rot_speed);
	old_plane_x = game->plane_x;
	game->plane_x = game->plane_x * cos(rot_speed) - game->plane_y * sin(rot_speed);
	game->plane_y = old_plane_x * sin(rot_speed) + game->plane_y * cos(rot_speed);
}

//進んだ先が０(床)か確認してから移動
//x,y別々に判定
void	move_player(t_game *game, double move_speed)
{
	if (game->world_map[(int)(game->pos_x + game->dir_x * move_speed)]
		[(int)game->pos_y] == 0)
		game->pos_x += game->dir_x * move_speed;
	if (game->world_map[(int)game->pos_x]
		[(int)(game->pos_y + game->dir_y * move_speed)] == 0)
		game->pos_y += game->dir_y * move_speed;
}

//左右移動用の関数
//move_speedがプラスなら右、マイナスなら左
void	move_strafe(t_game *game, double move_speed)
{
	if (game->world_map[(int)(game->pos_x - game->dir_y * move_speed)]
		[(int)game->pos_y] == 0)
	{
		game->pos_x += -game->dir_y * move_speed;
	}
	if (game->world_map[(int)game->pos_x]
		[(int)(game->pos_y + game->dir_x * move_speed)] == 0)
	{
		game->pos_y += game->dir_x * move_speed;
	}
}

// キー押下時: フラグをONにする
int	key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		close_window(game);
	if (keycode == KEY_W)
		game->key_w = 1;
	if (keycode == KEY_S)
		game->key_s = 1;
	if (keycode == KEY_A)
		game->key_a = 1;
	if (keycode == KEY_D)
		game->key_d = 1;
	if (keycode == KEY_LEFT)
		game->key_left = 1;
	if (keycode == KEY_RIGHT)
		game->key_right = 1;
	return (0);
}

// キーリリース時: フラグをOFFにする
int	key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->key_w = 0;
	if (keycode == KEY_S)
		game->key_s = 0;
	if (keycode == KEY_A)
		game->key_a = 0;
	if (keycode == KEY_D)
		game->key_d = 0;
	if (keycode == KEY_LEFT)
		game->key_left = 0;
	if (keycode == KEY_RIGHT)
		game->key_right = 0;
	return (0);
}

// game_loopから毎フレーム呼ばれる
// 押されているキーに応じて移動・回転を適用
void	process_movement(t_game *game)
{
	if (game->key_w)
		move_player(game, game->move_speed);
	if (game->key_s)
		move_player(game, -game->move_speed);
	if (game->key_d)
		move_strafe(game, game->move_speed);
	if (game->key_a)
		move_strafe(game, -game->move_speed);
	if (game->key_right)
		rotate_player(game, game->rot_speed);
	if (game->key_left)
		rotate_player(game, -game->rot_speed);
}
