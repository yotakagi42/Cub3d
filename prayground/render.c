/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 08:57:05 by ayamamot          #+#    #+#             */
/*   Updated: 2026/02/16 08:57:10 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = (char *)img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_floor_ceiling(t_game *game)
{
	int	x;
	int	y;
	int	c_color;
	int	f_color;

	c_color = 0x87CEEB;
	f_color = 0x8B4513;
	y = 0;
	while (y < SCREEN_H)
	{
		x = 0;
		while (x < SCREEN_W)
		{
			if (y < SCREEN_H / 2)
				my_mlx_pixel_put(&game->img, x, y, c_color);
			else
				my_mlx_pixel_put(&game->img, x, y, f_color);
			x++;
		}
		y++;
	}
}
