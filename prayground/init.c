#include "cub3d.h"

//引数からintの二次元配列のmapに保存するように変更
//t_game構造体のworld_mapを動的に確保する必要あり
//動的に確保して、map情報格納したものを構造体に入れ込む
static void	init_map(t_game *game)
{
	int	map[24][24] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	int	i;
	int	j;

	i = 0;
	game->map_w = 24;
	game->map_h = 24;
	while (i < game->map_w)
	{
		j = 0;
		while (j < game->map_h)
		{
			game->world_map[i][j] = map[i][j];
			j++;
		}
		i++;
	}
}

void	load_texture(t_game *game, int idx, char *path)
{
	t_img	*tex;

	tex = &game->textures[idx];
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Error\nFailed to load texture %s\n", path);
		exit(1);
	}
	tex->addr = (int *)mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->line_len, &tex->endian);
}

//N,S,E,Wがplayerの開始位置と向いている向き
//Nなら(dir_x,dir_y)=(0,-1),(plane_x,plane_y)=(0.66,0)
//Sなら(dir_x,dir_y)=(0,1),(plane_x,plane_y)=(-0.66,0)
//Eなら(dir_x,dir_y)=(1,0),(plane_x,plane_y)=(0)
//Wなら(dir_x,dir_y)=(-1,0),(plane_x,plane_y)=()
//speedの修正が必要(スペックに関わらず1秒で進む距離を一定にするため)
void	init_player(t_game *game)
{
	game->pos_x = 11.5;
	game->pos_y = 12.0;
	game->dir_x = -1.0;
	game->dir_y = 0.0;
	game->plane_x = 0.0;
	game->plane_y = -0.66;
	game->move_speed = 0.0;
	game->rot_speed = 0.0;
	game->frame_time = 0.0;
	game->key_w = 0;
	game->key_s = 0;
	game->key_a = 0;
	game->key_d = 0;
	game->key_left = 0;
	game->key_right = 0;
	gettimeofday(&game->old_time, NULL);
}

// frameTimeから移動量と回転量を計算
void	update_frame_time(t_game *game)
{
	double	old_ms;
	double	cur_ms;

	gettimeofday(&game->cur_time, NULL);
	old_ms = game->old_time.tv_sec * 1000.0 + game->old_time.tv_usec / 1000.0;
	cur_ms = game->cur_time.tv_sec * 1000.0 + game->cur_time.tv_usec / 1000.0;
	game->frame_time = (cur_ms - old_ms) / 1000.0;
	if (game->frame_time > 0.05)
		game->frame_time = 0.05;
	game->old_time = game->cur_time;
	game->move_speed = game->frame_time * 5.0;
	game->rot_speed = game->frame_time * 3.0;
}

void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		exit(1);
	init_map(game);
	init_player(game);
	game->win = mlx_new_window(game->mlx, SCREEN_W, SCREEN_H, "cub3D");
	if (!game->win)
		exit(1);
	game->img.img = mlx_new_image(game->mlx, SCREEN_W, SCREEN_H);
	game->img.addr = (int *)mlx_get_data_addr(game->img.img, &game->img.bpp,
			&game->img.line_len, &game->img.endian);
	load_texture(game, 0, "wall_n.xpm");
	load_texture(game, 1, "wall_s.xpm");
	load_texture(game, 2, "wall_w.xpm");
	load_texture(game, 3, "wall_e.xpm");
}
