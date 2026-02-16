/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 09:07:57 by ayamamot          #+#    #+#             */
/*   Updated: 2026/02/16 09:08:01 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
