/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 08:56:29 by ayamamot          #+#    #+#             */
/*   Updated: 2026/02/16 08:56:31 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
