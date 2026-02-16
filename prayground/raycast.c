/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 08:57:00 by ayamamot          #+#    #+#             */
/*   Updated: 2026/02/16 08:57:13 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_wall_stripe(t_game *game, t_ray *ray, int x)
{
	double	step;
	double	tex_pos;
	int		y;
	int		color;

	calc_tex_x(game, ray);
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
		
		ray.tex_num = 0;
		if (ray.side == 0 && ray.ray_dir_x > 0) ray.tex_num = 1;
		if (ray.side == 1 && ray.ray_dir_y < 0) ray.tex_num = 2;
		if (ray.side == 1 && ray.ray_dir_y > 0) ray.tex_num = 3;

		draw_wall_stripe(game, &ray, x);
		x++;
	}
}
