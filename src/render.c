/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/16 19:37:17 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/16 19:37:18 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

#define P_X(z) (points[z]->x)
#define P_Y(z) (points[z]->y)

static void		free_points(t_2dp *points[6])
{
	free(points[0]);
	free(points[1]);
	free(points[2]);
	free(points[3]);
	free(points[4]);
	free(points[5]);
}

static void		init_points(t_view *v, int x, t_2dp *points[6])
{
	double	camx;

	camx = 2.0 * (double)x / (double)WIN_WIDTH - 1.0;
	points[0] = ft_get_2d_point(v->player->pos->x, v->player->pos->y);
	points[1] = ft_get_2d_point(v->player->dir->x + v->player->cam->x * camx,
		v->player->dir->y + v->player->cam->y * camx);
	points[2] = ft_get_2d_point((int)P_X(0), (int)P_Y(0));
	points[4] = ft_get_2d_point(sqrt(1 + (P_Y(1) * P_Y(1)) / (P_X(1) * P_X(1))),
		sqrt(1 + (P_X(1) * P_X(1)) / (P_Y(1) * P_Y(1))));
	points[5] = ft_get_2d_point(P_X(1) < 0 ? -1 : 1, P_Y(1) < 0 ? -1 : 1);
	points[3] = ft_get_2d_point(P_X(1) < 0 ? (P_X(0) - P_X(2)) * P_X(4) :
		(P_X(2) + 1.0 - P_X(0)) * P_X(4),
		P_Y(1) < 0 ? (P_Y(0) - P_Y(2)) * P_Y(4) :
		(P_Y(2) + 1.0 - P_Y(0)) * P_Y(4));
}

static int		iter(t_view *v, t_2dp *points[6])
{
	int		hit_side;

	hit_side = 0;
	while (hit_side <= 0)
	{
		if (P_X(3) < P_Y(3))
		{
			P_X(3) += P_X(4);
			P_X(2) += P_X(5);
			hit_side = -1;
		}
		else
		{
			P_Y(3) += P_Y(4);
			P_Y(2) += P_Y(5);
			hit_side = -2;
		}
		if (v->map[(int)P_X(2)][(int)P_Y(2)] > 0)
			hit_side *= -1;
	}
	return (hit_side);
}

#define FL(x) ((int)(cur_floor[x] * T_SIZE) % T_SIZE)

static void		draw_floor(t_view *v, int hit_side, int y[4], t_2dp *points[6],
	double wall_dist)
{
	t_2dp	*floor_w;
	double	cur_floor[2];
	int		f_tex[2];
	t_color	color;

	floor_w = ft_get_2d_point(P_X(2), P_Y(2));
	if (hit_side == 1 && P_X(1) > 0)
		floor_w->y += P_X(3);
	else if (hit_side == 1 && P_X(1) < 0)
	{
		floor_w->x += 1.0;
		floor_w->y += P_X(3);
	}
	else if (hit_side == 2 && P_Y(1) > 0)
		floor_w->x += P_X(3);
	else
	{
		floor_w->x += P_X(3);
		floor_w->y += 1.0;
	}
	y[2] = y[2] < 0 ? WIN_HEIGHT : y[2];
	while (++y[2] < WIN_HEIGHT)
	{
		double weight = v->tab[y[2]] / wall_dist;
		cur_floor[0] = weight * floor_w->x + (1.0 - weight) *
			v->player->pos->x;
		cur_floor[1] = weight * floor_w->y + (1.0 - weight) *
			v->player->pos->y;
		f_tex[0] = FL(0);
		f_tex[1] = FL(1);
		color = v->textures[78][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		v->pixels[y[2] * v->size_line + (y[3] * 4)] = color;
		v->pixels[y[2] * v->size_line + (y[3] * 4) + 1] = color >> 8;
		v->pixels[y[2] * v->size_line + (y[3] * 4) + 2] = color >> 16;
		color = v->textures[2][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		v->pixels[(WIN_HEIGHT - y[2] - 1) * v->size_line + (y[3] * 4)] = color;
		v->pixels[(WIN_HEIGHT - y[2] - 1) * v->size_line + (y[3] * 4) + 1] = color >> 8;
		v->pixels[(WIN_HEIGHT - y[2] - 1) * v->size_line + (y[3] * 4) + 2] = color >> 16;
	}
}

static void		draw_stripe(t_view *v, int y[4], int tex[3], int hit_side)
{
	int		iy;
	int		d;
	t_color	color;

	iy = y[1] - 1;
	while (++iy < y[2])
	{
		d = iy * 0x100 - WIN_HEIGHT * 0x80 + y[0] * 0x80;
		tex[2] = ((d * T_SIZE) / y[0]) / 0x100;
		color = v->textures[tex[0]][T_SIZE * tex[2] + tex[1]];
		color = hit_side == 2 ? (color >> 1) & 0x7F7F7F : color;
		v->pixels[iy * v->size_line + (y[3] * 4)] = color;
		v->pixels[iy * v->size_line + (y[3] * 4) + 1] = color >> 8;
		v->pixels[iy * v->size_line + (y[3] * 4) + 2] = color >> 16;
	}
}

/*
**	Draws a vertical stripe for a given x position in the screen. For a given x
**	coordinate, draws a ray in that direction, calculates the first intersection
**	with a wall, then draws a vertical line depending on the distance to that
**	place. points is an array out of space savings which contains several
**	distinct points for the calculations. points[x] descriptions:
**	 * [0] -- Ray Position (starting at player's coordinates;
**	 * [1] -- Ray Direction (constant vector for the ray)
**	 * [2] -- Map Position (more of an integer, specific grid square)
**	 * [3] -- Side Distance (distance until next integer x or y coordinate)
**	 * [4] -- Delta Distance (how long it takes between one integer coordinate
**			and the next one)
**	 * [5] -- Step (what direction in both coordinates we are going)
**	Hit side is a variable that both checks whether or not a wall has been hit,
**	and if so, which side it hit (1=EW, 2=NS). While nothing has been hit, it is
**	a negative number.
**	DISCLAIMER: This is awful. I do this to satisfy the Norm, which 42 makes us
**	follow. That said, I at least made this comment to not make it actual hell
**	to look at.
*/

void			render_column(t_view *v, int x)
{
	t_2dp	*points[6];
	double	wall_dist;
	int		y[4];
	int		tex[3];
	int		hit_side;

	init_points(v, x, points);
	hit_side = iter(v, points);
	if (hit_side == 1)
		wall_dist = (P_X(2) - P_X(0) + (1 - P_X(5)) / 2.0) / P_X(1);
	else
		wall_dist = (P_Y(2) - P_Y(0) + (1 - P_Y(5)) / 2.0) / P_Y(1);
	y[0] = (int)(WIN_HEIGHT / wall_dist);
	y[1] = WIN_HEIGHT / 2 - y[0] / 2;
	y[1] = y[1] < 0 ? 0 : y[1];
	y[2] = y[0] / 2 + WIN_HEIGHT / 2;
	y[2] = y[2] > WIN_HEIGHT ? WIN_HEIGHT : y[2];
	y[3] = x;
	tex[0] = v->map[(int)P_X(2)][(int)P_Y(2)] - 1;
	P_X(3) = (hit_side == 1) ? P_Y(0) + wall_dist * P_Y(1) : P_X(0) +
		wall_dist * P_X(1);
	P_X(3) -= floor(P_X(3));
	tex[1] = (int)(P_X(3) * (double) T_SIZE);
	if ((hit_side == 1 && P_X(1) > 0) || (hit_side == 2 && P_Y(1) < 0))
		tex[1] = T_SIZE - tex[1] - 1;
	draw_stripe(v, y, tex, hit_side);
	draw_floor(v, hit_side, y, points, wall_dist);
	free_points(points);
}
