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

static void		init_points(t_view *v, t_render *r)
{
	r->camx = 2.0 * (double)r->x / (double)WIN_WIDTH - 1.0;
	r->rayposx = v->player->pos->x;
	r->rayposy = v->player->pos->y;
	r->raydirx = v->player->dir->x + v->player->cam->x * r->camx;
	r->raydiry = v->player->dir->y + v->player->cam->y * r->camx;
	r->mapx = (int)r->rayposx;
	r->mapy = (int)r->rayposy;
	r->deltadistx = sqrt(1 + (r->raydiry * r->raydiry) /
		(r->raydirx * r->raydirx));
	r->deltadisty = sqrt(1 + (r->raydirx * r->raydirx) /
		(r->raydiry * r->raydiry));
	r->stepx = r->raydirx < 0 ? -1 : 1;
	r->stepy = r->raydiry < 0 ? -1 : 1;
	r->sidedistx = r->raydirx < 0 ? (r->rayposx - r->mapx) * r->deltadistx :
		(r->mapx + 1.0 - r->rayposx) * r->deltadistx,
	r->sidedisty = r->raydiry < 0 ? (r->rayposy - r->mapy) * r->deltadisty :
		(r->mapy + 1.0 - r->rayposy) * r->deltadisty;
}

static void		iter(t_view *v, t_render *r)
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
		if (v->map[(int)r->mapx][(int)r->mapy] > 0)
			r->hit = 1;
	}
}

#define FL(x) ((int)(cur_floor[x] * T_SIZE) % T_SIZE)

static void		darken_floor(t_view *v, t_render *r)
{
	r->red = (r->color >> 16) * v->darkness[r->y] * 0.75;
	r->green = ((r->color >> 8) & 0xFF) * v->darkness[r->y] * 0.75;
	r->blue = (r->color & 0xFF) * v->darkness[r->y] * 0.75;
	r->color = ((int)r->red) << 16 | ((int)r->green) << 8 | (int)r->blue;
}

static void		draw_floor(t_view *v, t_render *r)
{
	t_2dp	*floor_w;
	double	cur_floor[2];
	int		f_tex[2];

	floor_w = ft_get_2d_point(r->mapx, r->mapy);
	if (r->side == 0 && r->raydirx > 0)
		floor_w->y += r->wallx;
	else if (r->side == 0 && r->raydirx < 0)
	{
		floor_w->x += 1.0;
		floor_w->y += r->wallx;
	}
	else if (r->side == 1 && r->raydiry > 0)
		floor_w->x += r->wallx;
	else
	{
		floor_w->x += r->wallx;
		floor_w->y += 1.0;
	}
	r->y = r->drawend < 0 ? WIN_HEIGHT : r->drawend;
	while (++(r->y) < WIN_HEIGHT)
	{
		double weight = v->tab[r->y] / r->walldist;
		cur_floor[0] = weight * floor_w->x + (1.0 - weight) *
			v->player->pos->x;
		cur_floor[1] = weight * floor_w->y + (1.0 - weight) *
			v->player->pos->y;
		f_tex[0] = FL(0);
		f_tex[1] = FL(1);
		r->color = v->textures[78][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		darken_floor(v, r);
		v->pixels[r->y * v->size_line + (r->x * 4)] = r->color;
		v->pixels[r->y * v->size_line + (r->x * 4) + 1] = r->color >> 8;
		v->pixels[r->y * v->size_line + (r->x * 4) + 2] = r->color >> 16;
		r->color = v->textures[2][(int)(T_SIZE * f_tex[1] + f_tex[0])];
		darken_floor(v, r);
		v->pixels[(WIN_HEIGHT - r->y - 1) * v->size_line + (r->x * 4)] = r->color;
		v->pixels[(WIN_HEIGHT - r->y - 1) * v->size_line + (r->x * 4) + 1] = r->color >> 8;
		v->pixels[(WIN_HEIGHT - r->y - 1) * v->size_line + (r->x * 4) + 2] = r->color >> 16;
	}
	free(floor_w);
} // TODO TODO

static void		darken_wall(t_view *v, t_render *r)
{
	if (r->lineheight >= WIN_HEIGHT)
		return ;
	r->red = (r->color >> 16) * (v->darkness[r->lineheight]);
	r->green = ((r->color >> 8) & 0xFF) * (v->darkness[r->lineheight]);
	r->blue = (r->color & 0xFF) * (v->darkness[r->lineheight]);
	r->color = ((int)r->red) << 16 | ((int)r->green) << 8 | (int)r->blue;
}

static void		draw_stripe(t_view *v, t_render *r)
{
	int		d;

	r->y = r->drawstart - 1;
	while (++r->y < r->drawend)
	{
		d = r->y * 0x100 - WIN_HEIGHT * 0x80 + r->lineheight * 0x80;
		r->texy = ((d * T_SIZE) / r->lineheight) / 0x100;
		r->color = v->textures[r->texnum][T_SIZE * r->texy + r->texx];
		r->color = r->side == 1 ? (r->color >> 1) & 0x7F7F7F : r->color;
		darken_wall(v, r);
		v->pixels[r->y * v->size_line + (r->x * 4)] = r->color;
		v->pixels[r->y * v->size_line + (r->x * 4) + 1] = r->color >> 8;
		v->pixels[r->y * v->size_line + (r->x * 4) + 2] = r->color >> 16;
	}
}

void			render_column(t_view *v, t_render *r)
{
	init_points(v, r);
	iter(v, r);
	if (r->side == 0)
		r->walldist = (r->mapx - r->rayposx + (1 - r->stepx) / 2.0) / r->raydirx;
	else
		r->walldist = (r->mapy - r->rayposy + (1 - r->stepy) / 2.0) / r->raydiry;
	r->lineheight = (int)(WIN_HEIGHT / r->walldist);
	r->drawstart = WIN_HEIGHT / 2 - r->lineheight / 2;
	r->drawstart = r->drawstart < 0 ? 0 : r->drawstart;
	r->drawend = r->lineheight / 2 + WIN_HEIGHT / 2;
	r->drawend = r->drawend > WIN_HEIGHT ? WIN_HEIGHT : r->drawend;
	r->texnum = v->map[(int)r->mapx][(int)r->mapy] - 1;
	r->wallx = (r->side == 0) ? r->rayposy + r->walldist * r->raydiry :
		r->rayposx + r->walldist * r->raydirx;
	r->wallx -= floor(r->wallx);
	r->texx = (int)(r->wallx * (double) T_SIZE);
	if ((r->side == 0 && r->raydirx > 0) || (r->side == 1 && r->raydiry < 0))
		r->texx = T_SIZE - r->texx - 1;
	draw_stripe(v, r);
	draw_floor(v, r);
}


void			draw_reload(t_view *view)
{
	int			x;
	t_render	*r;

	r = (t_render*)ft_memalloc(sizeof(t_render));
	view->img = mlx_new_image(view->id, WIN_WIDTH + 100, WIN_HEIGHT + 100);
	view->pixels = mlx_get_data_addr(view->img, &(view->bits_per_pixel),
		&(view->size_line), &(view->endian));
	x = -1;
	while (++x < WIN_WIDTH)
	{
		r->x = x;
		render_column(view, r);
	}
	mlx_put_image_to_window(view->id, view->win, view->img, 0, 0);
	mlx_destroy_image(view->id, view->img);
	free(r);
}

int				expose_hook(t_view *view)
{
	draw_reload(view);
	return (0);
}
