/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dungeon.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/18 16:33:31 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/18 16:33:32 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		get_ready(t_view *v)
{
	int	y;

	y = -1;
	v->tab = (double*)ft_memalloc(sizeof(double) * WIN_HEIGHT);
	v->darkness = (double*)ft_memalloc(sizeof(double) * WIN_HEIGHT);
	v->map = (char**)ft_memalloc(sizeof(char*) * MAP_HEIGHT);
	v->zbuffer = (double*)ft_memalloc(sizeof(double) * WIN_WIDTH);
	v->sprites = (t_sprite*)ft_memalloc(sizeof(t_sprite) * 2);
	v->sprites[0].x = MAP_WIDTH / 2 + 2.01;
	v->sprites[0].y = MAP_HEIGHT / 2 + 2.01;
	v->sprites[0].texnum = 93;
	v->sprites[1].x = MAP_WIDTH / 2 - 2.01;
	v->sprites[1].y = MAP_HEIGHT / 2 - 2.01;
	v->sprites[1].texnum = 93;
	while (++y < MAP_HEIGHT)
		v->map[y] = (char*)ft_memalloc(sizeof(char) * MAP_WIDTH);
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		v->tab[y] = 2.0 * WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
		v->darkness[y] = MAX(0, MIN(1, (double)y / (double)WIN_HEIGHT) + 0.15);
	}
	v->pressed = (t_keys*)ft_memalloc(sizeof(t_keys));
	v->cur_time = 0;
	v->running = 1;
	make_starting_room(v);
	srand(time(NULL));
}

static int		bust_wall_hor(t_view *v, int r, int c)
{
	int sc;

	sc = c;
	while (v->map[r - 1][c] && v->map[r + 1][c] && !v->map[r][c])
		c--;
	if (v->map[r][c])
		return (0);
	c = sc;
	while (v->map[r - 1][c] && v->map[r + 1][c] && !v->map[r][c])
		c++;
	if (v->map[r][c])
		return (0);
	c = sc;
	v->map[r][c] = -3;
	return (1);
}

static int		bust_wall_ver(t_view *v, int r, int c)
{
	int sr;

	sr = r;
	while (v->map[r][c - 1] && v->map[r][c + 1] && !v->map[r][c])
		r--;
	if (v->map[r][c])
		return (0);
	r = sr;
	while (v->map[r][c - 1] && v->map[r][c + 1] && !v->map[r][c])
		r++;
	if (v->map[r][c])
		return (0);
	r = sr;
	v->map[r][c] = -3;
	return (1);
}

static void		break_walls(t_view *v)
{
	int		r;
	int		c;
	int		dir;
	int		access;
	int		i;

	access = 0;
	i = 0;
	while (access < ACCESS_NUM && i++ < 100000)
	{
		r = rand() % (MAP_HEIGHT - 2) + 1;
		c = rand() % (MAP_WIDTH - 2) + 1;
		dir = 0;
		if (v->map[r][c])
			continue ;
		if (v->map[r + 1][c])
			dir += 1;
		if (v->map[r][c - 1])
			dir += 2;
		if (v->map[r - 1][c])
			dir += 4;
		if (v->map[r][c + 1])
			dir += 8;
		if (dir == 5)
		{
			if (bust_wall_hor(v, r, c))
				access++;
		}
		else if (dir == 10)
		{
			if (bust_wall_ver(v, r, c))
				access++;
		}
	}
}

static int		gen_something(t_view *view, t_point *p)
{
	int		picker;

	picker = rand() % 100 + 1;
	if (picker < ROOM_DEN)
		return (gen_room(view, p));
	else
		return (gen_corr(view, p));
}

static int		check_wall(t_view *v, t_point *p)
{
	if (v->map[p->r][p->c] != 0)
	{
		free(p);
		return (0);
	}
	if (find_dir(v, p) == -1)
	{
		free(p);
		return (0);
	}
	return (1);
}

void			gen_dungeon(t_view *view)
{
	int		r;
	int		c;
	t_point	*p;
	int		built_num;
	int		stopper[2];

	get_ready(view);
	built_num = 0;
	stopper[1] = 0;
	while (built_num < BUILD_NUM && stopper[1] < 100)
	{
		stopper[0] = built_num;
		r = rand() % (MAP_HEIGHT - 2) + 1;
		c = rand() % (MAP_WIDTH - 2) + 1;
		p = get_point(c, r);
		if (!check_wall(view, p))
			continue ;
		if (gen_something(view, p))
			built_num++;
		free(p);
		stopper[1] = stopper[0] == built_num ? stopper[1] + 1 : 0;
	}
	if (stopper[1] >= 100)
		map_error();
	break_walls(view);
}
