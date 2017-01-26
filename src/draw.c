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

#define P_DX v->player->dir->x
#define P_DY v->player->dir->y

#define ABS(x) ((x) < 0 ? -(x) : (x))

void		draw_floor(t_view *v, t_render *r, t_2dp *floor_w, double weight)
{
	double	cur_floor[2];
	int		f_tex[2];
	int		texnum;
	int		modx;
	int		mody;
	int		prev;

	init_floor(r, &floor_w);
	texnum = -42;
	while (++(r->y) < W_H)
	{
		weight = v->tab[r->y] / r->walldist;
		cur_floor[0] = weight * floor_w->x + (1.0 - weight) *
			v->player->pos->x;
		cur_floor[1] = weight * floor_w->y + (1.0 - weight) *
			v->player->pos->y;
		if (texnum == -42)
		{
			if (v->map[(int)cur_floor[1] + (P_DY > 0 ? 1 : -1)]
				[(int)cur_floor[0]] > 0)
				texnum = v->map[(int)cur_floor[1] + (P_DY > 0 ? 1 : -1)]
					[(int)cur_floor[0]] + 35;
			else if (v->map[(int)cur_floor[1]]
				[(int)cur_floor[0] + (P_DX > 0 ? 1 : -1)] > 0)
				texnum = v->map[(int)cur_floor[1]]
					[(int)cur_floor[0] + (P_DX > 0 ? 1 : -1)] + 35;
			else
				texnum = 35;
		}
		f_tex[0] = FL(0);
		f_tex[1] = FL(1);
		prev = texnum;
		texnum = v->map[(int)cur_floor[1]][(int)cur_floor[0]] + 35;
		if (texnum < 35)
		{
			texnum = prev;
		}
		r->color = v->textures[texnum][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		r->color = ft_color_mult(r->color, 1.0 / v->tab[r->y]);
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
		}
	}
}
