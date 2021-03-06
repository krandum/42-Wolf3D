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

static void		darken_wall(t_view *v, t_render *r)
{
	if (r->lineheight < W_H)
		r->color = ft_color_mult(r->color, v->darkness[r->lineheight]);
}

static void		draw_stripe(t_view *v, t_render *r)
{
	int		d;

	r->y = r->drawstart - 1;
	while (++r->y < r->drawend)
	{
		d = r->y * 0x100 - W_H * 0x80 + r->lineheight * 0x80;
		r->texy = ((d * T_SIZE) / r->lineheight) / 0x100;
		r->color = v->textures[r->texnum][T_SIZE * r->texy + r->texx];
		if (r->color == 0xFF00FF)
			continue ;
		if (r->lineheight < W_H)
			r->color = ft_color_mult(r->color, v->darkness[r->lineheight]);
		v->pixels[r->y * v->s_line + (r->x * 4)] = r->color;
		v->pixels[r->y * v->s_line + (r->x * 4) + 1] = r->color >> 8;
		v->pixels[r->y * v->s_line + (r->x * 4) + 2] = r->color >> 16;
	}
}

#define P_DX v->player->dir->x
#define P_DY v->player->dir->y

static void		set_texnum(t_view *v, t_render *r)
{
	int			adj;
	int			modx;
	int			mody;

	modx = 0;
	mody = 0;
	if (r->side == 0 && r->raydx > 0)
		modx = -1;
	else if (r->side == 0 && r->raydx < 0)
		modx = 1;
	else if (r->side == 1 && r->raydy > 0)
		mody = -1;
	else
		mody = 1;
	adj = v->map[(int)r->mapy + mody][(int)r->mapx + modx];
	r->texnum = adj - 1;
	if (r->texnum < 0)
	{
		if (v->map[(int)r->mapy + mody + (P_DY > 0 ? 1 : -1)]
			[(int)r->mapx + modx] > 0)
			r->texnum = v->map[(int)r->mapy + mody + (P_DY > 0 ? 1 : -1)]
				[(int)r->mapx + modx] - 1;
		else
			r->texnum = v->map[(int)r->mapy + mody]
				[(int)r->mapx + modx + (P_DX > 0 ? 1 : -1)] - 1;
	}
	if (r->texnum < 0)
		r->texnum = 8;
	if (v->map[(int)r->mapy][(int)r->mapx] < 0)
		r->texnum += -9 * v->map[(int)r->mapy][(int)r->mapx];
}

void			render_column(t_view *v, t_render *r)
{
	init_points(v, r);
	iter(v, r);
	if (r->side == 0)
		r->walldist = (r->mapx - r->rayposx + (1 - r->stepx) / 2.0) / r->raydx;
	else
		r->walldist = (r->mapy - r->rayposy + (1 - r->stepy) / 2.0) / r->raydy;
	r->lineheight = (int)(2.0 * W_H / r->walldist);
	r->drawstart = W_H / 2 - r->lineheight / 2;
	r->drawstart = r->drawstart < 0 ? 0 : r->drawstart;
	r->drawend = r->lineheight / 2 + W_H / 2;
	r->drawend = r->drawend > W_H ? W_H : r->drawend;
	set_texnum(v, r);
	r->wallx = (r->side == 0) ? r->rayposy + r->walldist * r->raydy :
		r->rayposx + r->walldist * r->raydx;
	r->wallx -= floor(r->wallx);
	r->texx = (int)(r->wallx * (double)T_SIZE);
	if ((r->side == 0 && r->raydx > 0) || (r->side == 1 && r->raydy < 0))
		r->texx = T_SIZE - r->texx - 1;
	draw_stripe(v, r);
	v->zbuffer[r->x] = r->walldist;
	draw_floor(v, r, NULL, 0.0);
	if (r->found_door)
	{
		r->mapx = r->found_door->mapx;
		r->mapy = r->found_door->mapy;
		r->side = r->found_door->side;
		if (r->side == 0)
			r->walldist = (r->mapx - r->rayposx + (1 - r->stepx) / 2.0) / r->raydx;
		else
			r->walldist = (r->mapy - r->rayposy + (1 - r->stepy) / 2.0) / r->raydy;
		r->lineheight = (int)(2.0 * W_H / r->walldist);
		r->drawstart = W_H / 2 - r->lineheight / 2;
		r->drawstart = r->drawstart < 0 ? 0 : r->drawstart;
		r->drawend = r->lineheight / 2 + W_H / 2;
		r->drawend = r->drawend > W_H ? W_H : r->drawend;
		set_texnum(v, r);
		r->wallx = (r->side == 0) ? r->rayposy + r->walldist * r->raydy :
			r->rayposx + r->walldist * r->raydx;
		r->wallx -= floor(r->wallx);
		r->texx = (int)(r->wallx * (double)T_SIZE);
		if ((r->side == 0 && r->raydx > 0) || (r->side == 1 && r->raydy < 0))
			r->texx = T_SIZE - r->texx - 1;
		draw_stripe(v, r);
		free(r->found_door);
		r->found_door = 0;
	}
}

static void		*threaded_artist(void *tmp)
{
	int			x;
	t_split		*s;

	s = (t_split*)tmp;
	pthread_mutex_lock(&s->view->mutex);
	pthread_cond_wait(&s->view->signal_cond, &s->view->mutex);
	pthread_mutex_unlock(&s->view->mutex);
	while (s->view->running)
	{
		ft_bzero(s->render, sizeof(t_render));
		x = s->x_start - 1;
		while (++x < s->x_end)
		{
			s->render->x = x;
			render_column(s->view, s->render);
		}
		pthread_mutex_lock(&s->view->mutex);
		s->view->running_threads--;
		if (s->view->running_threads == 0)
			pthread_cond_signal(&s->view->done_cond);
		pthread_mutex_unlock(&s->view->mutex);
		pthread_mutex_lock(&s->view->mutex);
		pthread_cond_wait(&s->view->signal_cond, &s->view->mutex);
		pthread_mutex_unlock(&s->view->mutex);
	}
	free(s->render);
	return (NULL);
}

static void		put_fist(t_view *v)
{
	int		x;
	int		y;
	float	r;
	float	c;
	t_color	color;

	r = 0.0;
	y = WIN_HEIGHT * 2 / 3 - 1;
	while (++y < WIN_HEIGHT)
	{
		c = 0.0;
		x = WIN_WIDTH / 4 - 1;
		while (++x < WIN_WIDTH * 3 / 4)
		{
			color = v->fists[v->pressed->punching]
				[256 * (int)r + (int)c];
			c += 256.0 / (float)(WIN_WIDTH / 2);
			if (color == 0xFF00FF)
				continue ;
			v->pixels[y * v->s_line + (x * 4)] = color;
			v->pixels[y * v->s_line + (x * 4) + 1] = color >> 8;
			v->pixels[y * v->s_line + (x * 4) + 2] = color >> 16;
		}
		r += 128.0 / (float)(WIN_HEIGHT / 3);
	}
	if (v->pressed->punching < 3 && v->pressed->punching)
		v->pressed->punching++;
}

void			draw_reload(t_view *view)
{
	static t_split	*splits = 0;
	int				i;

	view->img = mlx_new_image(view->id, WIN_WIDTH + 100, W_H + 100);
	view->pixels = mlx_get_data_addr(view->img, &(view->bits_per_pixel),
		&(view->s_line), &(view->endian));
	if (!splits)
	{
		splits = (t_split*)ft_memalloc(sizeof(t_split) * 20);
		i = -1;
		pthread_mutex_init(&view->mutex, NULL);
		pthread_cond_init(&view->signal_cond, NULL);
		pthread_cond_init(&view->done_cond, NULL);
		view->running_threads = 20;
		while (++i < 20)
		{
			splits[i].x_start = (WIN_WIDTH / 20) * i;
			splits[i].x_end = (WIN_WIDTH / 20) * (i + 1);
			splits[i].render = (t_render*)malloc(sizeof(t_render));
			splits[i].view = view;
			pthread_create(&(splits[i].thread), NULL, threaded_artist,
				(void*)&splits[i]);
		}
		sleep(0);
	}
	pthread_mutex_lock(&view->mutex);
	view->running_threads = 20;
	pthread_cond_broadcast(&view->signal_cond);
	pthread_mutex_unlock(&view->mutex);
	pthread_mutex_lock(&view->mutex);
	pthread_cond_wait(&view->done_cond, &view->mutex);
	pthread_mutex_unlock(&view->mutex);
	put_fist(view);
	mlx_put_image_to_window(view->id, view->win, view->img, 0, 0);
	mlx_destroy_image(view->id, view->img);
}
