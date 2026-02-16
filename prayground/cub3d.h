/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 08:55:27 by ayamamot          #+#    #+#             */
/*   Updated: 2026/02/16 08:55:29 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "minilibx-linux/mlx.h"
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
#include <X11/keysym.h>

# define SCREEN_W 640
# define SCREEN_H 480
# define TEX_W 64
# define TEX_H 64

# define KEY_ESC XK_Escape
# define KEY_W XK_w
# define KEY_A XK_a
# define KEY_S XK_s
# define KEY_D XK_d
# define KEY_LEFT XK_Left
# define KEY_RIGHT XK_Right

typedef struct s_img
{
	void	*img;
	int		*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_img	img;
	t_img	textures[4];
	
	// Map
	int		world_map[24][24];
	int		map_w;
	int		map_h;
	//下の方を使う
	// int		**world_map;

	// Player
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;

	// Movement
	double	move_speed;
	double	rot_speed;

	// Time
	struct timeval	old_time;
	struct timeval	cur_time;
	double	frame_time;

	// Key states (1=pressed, 0=released)
	int		key_w;
	int		key_s;
	int		key_a;
	int		key_d;
	int		key_left;
	int		key_right;
}	t_game;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	int		tex_x;
}	t_ray;

// init.c
void	init_game(t_game *game);
void	load_texture(t_game *game, int idx, char *path);
void	update_frame_time(t_game *game);

// move.c
void	rotate_player(t_game *game, double rot_speed);
void	move_player(t_game *game, double move_speed);
void	move_strafe(t_game *game, double move_speed);

// input.c
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
void	process_movement(t_game *game);

// render.c
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void	draw_floor_ceiling(t_game *game);

// raycast.c
void	raycasting(t_game *game);

// utils_ray.c
void	init_ray(t_game *game, t_ray *ray, int x);
void	calc_step(t_game *game, t_ray *ray);
void	perform_dda(t_game *game, t_ray *ray);
void	calc_wall_dist(t_ray *ray);
void	calc_tex_x(t_game *game, t_ray *ray);

// close.c
int		close_window(t_game *game);
#endif
