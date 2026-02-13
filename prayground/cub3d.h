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

// init.c
void	init_game(t_game *game);
void	load_texture(t_game *game, int idx, char *path);
void	update_frame_time(t_game *game);

// input.c
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_game *game);
void	process_movement(t_game *game);
int		close_window(t_game *game);

// render.c
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void	draw_floor_ceiling(t_game *game);

// raycast.c
void	raycasting(t_game *game);

#endif
