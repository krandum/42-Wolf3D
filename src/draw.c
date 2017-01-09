/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 09:45:39 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/24 09:45:40 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

#define FL(x) ((int)(cur_floor[x] * T_SIZE) % T_SIZE)

static void	darken_floor(t_view *v, t_render *r)
{
	r->red = (r->color >> 16) * v->darkness[r->y] * 0.75;
	r->green = ((r->color >> 8) & 0xFF) * v->darkness[r->y] * 0.75;
	r->blue = (r->color & 0xFF) * v->darkness[r->y] * 0.75;
	r->color = ((int)r->red) << 16 | ((int)r->green) << 8 | (int)r->blue;
}

static void	init_floor(t_render *r, t_2dp **floor_w)
{
	(*floor_w) = ft_get_2d_point(r->mapx, r->mapy);
	if (r->side == 0 && r->raydx > 0)
		(*floor_w)->y += r->wallx;
	else if (r->side == 0 && r->raydx < 0)
	{
		(*floor_w)->x += 1.0;
		(*floor_w)->y += r->wallx;
	}
	else if (r->side == 1 && r->raydy > 0)
		(*floor_w)->x += r->wallx;
	else
	{
		(*floor_w)->x += r->wallx;
		(*floor_w)->y += 1.0;
	}
	r->y = r->drawend < 0 ? W_H : r->drawend;
}

void		draw_floor(t_view *v, t_render *r, t_2dp *floor_w, double weight)
{
	double	cur_floor[2];
	int		f_tex[2];

	init_floor(r, &floor_w);
	while (++(r->y) < W_H)
	{
		weight = v->tab[r->y] / r->walldist;
		cur_floor[0] = weight * floor_w->x + (1.0 - weight) *
			v->player->pos->x;
		cur_floor[1] = weight * floor_w->y + (1.0 - weight) *
			v->player->pos->y;
		f_tex[0] = FL(0);
		f_tex[1] = FL(1);
		r->color = v->textures[72][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		darken_floor(v, r);
		v->pixels[r->y * v->s_line + (r->x * 4)] = r->color;
		v->pixels[r->y * v->s_line + (r->x * 4) + 1] = r->color >> 8;
		v->pixels[r->y * v->s_line + (r->x * 4) + 2] = r->color >> 16;
	}
	free(floor_w);
}

void		init_points(t_view *v, t_render *r)
{
	r->camx = 2.0 * (double)r->x / (double)WIN_WIDTH - 1.0;
	r->rayposx = v->player->pos->x;
	r->rayposy = v->player->pos->y;
	r->raydx = v->player->dir->x + v->player->cam->x * r->camx;
	r->raydy = v->player->dir->y + v->player->cam->y * r->camx;
	r->mapx = (int)r->rayposx;
	r->mapy = (int)r->rayposy;
	r->deltadistx = sqrt(1 + (r->raydy * r->raydy) /
		(r->raydx * r->raydx));
	r->deltadisty = sqrt(1 + (r->raydx * r->raydx) /
		(r->raydy * r->raydy));
	r->stepx = r->raydx < 0 ? -1 : 1;
	r->stepy = r->raydy < 0 ? -1 : 1;
	r->sidedistx = r->raydx < 0 ? (r->rayposx - r->mapx) * r->deltadistx :
		(r->mapx + 1.0 - r->rayposx) * r->deltadistx;
	r->sidedisty = r->raydy < 0 ? (r->rayposy - r->mapy) * r->deltadisty :
		(r->mapy + 1.0 - r->rayposy) * r->deltadisty;
}

void		iter(t_view *v, t_render *r)
{
	r->hit = 0;
	while (r->hit == 0)
	{
		if (r->sidedistx < r->sidedisty)
		{
			r->sidedistx += r->deltadistx;
			r->mapx += r->stepx;
			r->side = 0;
		}
		else
		{
			r->sidedisty += r->deltadisty;
			r->mapy += r->stepy;
			r->side = 1;
		}
		if (v->map[(int)r->mapy][(int)r->mapx] == 0
			|| (r->found_door && v->map[(int)r->mapy][(int)r->mapx] < 0))
			r->hit = 1;
		if (v->map[(int)r->mapy][(int)r->mapx] < 0 && r->found_door == 0)
		{
			r->found_door = (t_door*)ft_memalloc(sizeof(t_door));
			r->found_door->mapx = r->mapx;
			r->found_door->mapy = r->mapy;
			r->found_door->side = r->side;
			r->found_door->sidedistx = r->sidedistx;
			r->found_door->sidedisty = r->sidedisty;
		}
	}
}
