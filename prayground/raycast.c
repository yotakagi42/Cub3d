#include "cub3d.h"

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

static void	init_ray(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)SCREEN_W - 1;
	ray->ray_dir_x = game->dir_x + game->plane_x * ray->camera_x;
	ray->ray_dir_y = game->dir_y + game->plane_y * ray->camera_x;
	ray->map_x = (int)game->pos_x;
	ray->map_y = (int)game->pos_y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
}

static void	calc_step(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->pos_y) * ray->delta_dist_y;
	}
}

static void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->map_w
			|| ray->map_y < 0 || ray->map_y >= game->map_h
			|| game->world_map[ray->map_x][ray->map_y] > 0)
			ray->hit = 1;
	}
}

static void	calc_wall_dist(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
	else
		ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
	ray->line_height = (int)(SCREEN_H / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + SCREEN_H / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + SCREEN_H / 2;
	if (ray->draw_end >= SCREEN_H)
		ray->draw_end = SCREEN_H - 1;
}

static void	draw_wall_stripe(t_game *game, t_ray *ray, int x)
{
	double	wall_x;
	double	step;
	double	tex_pos;
	int		y;
	int		color;

	if (ray->side == 0)
		wall_x = game->pos_y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		wall_x = game->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
	wall_x -= floor(wall_x);
	ray->tex_x = (int)(wall_x * (double)TEX_W);
	if ((ray->side == 0 && ray->ray_dir_x > 0)
		|| (ray->side == 1 && ray->ray_dir_y < 0))
		ray->tex_x = TEX_W - ray->tex_x - 1;
	step = 1.0 * TEX_H / ray->line_height;
	tex_pos = (ray->draw_start - SCREEN_H / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		color = game->textures[ray->tex_num].addr[TEX_H * ((int)tex_pos
				& (TEX_H - 1)) + ray->tex_x];
		tex_pos += step;
		my_mlx_pixel_put(&game->img, x, y, color);
		y++;
	}
}

void	raycasting(t_game *game)
{
	int		x;
	t_ray	ray;

	x = 0;
	while (x < SCREEN_W)
	{
		init_ray(game, &ray, x);
		calc_step(game, &ray);
		perform_dda(game, &ray);
		calc_wall_dist(&ray);
		
		// Map texture based on side/direction
		ray.tex_num = 0;
		if (ray.side == 0 && ray.ray_dir_x > 0) ray.tex_num = 1;
		if (ray.side == 1 && ray.ray_dir_y < 0) ray.tex_num = 2;
		if (ray.side == 1 && ray.ray_dir_y > 0) ray.tex_num = 3;

		draw_wall_stripe(game, &ray, x);
		x++;
	}
}
