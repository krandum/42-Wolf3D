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

#define C(x) (i == 0 || i == p1->x - p0->x - 1)

static t_point	*get_point(int r, int c)
{
	t_point	*out;

	out = (t_point*)ft_memalloc(sizeof(t_point));
	out->r = r;
	out->c = c;
	return (out);
}

static int		check_space(t_view *v, t_point *p0, t_point *p1)
{
	int		r;
	int		c;

	if (p0->r > WIN_HEIGHT || p1->r > WIN_HEIGHT || p1->r < 0 || p0->r < 0)
		return (0);
	if (p0->c > WIN_WIDTH || p1->c > WIN_WIDTH || p1->c < 0 || p0->c < 0)
		return (0);
	r = p0->r;
	while (r <= p1->r)
	{
		c = p0->c;
		while (c <= p1->c)
		{
			if (v->map[r][c] != 0)
				return (0);
			c++;
		}
		r++;
	}
	return (1);
}

static int		make_room(t_view *v, t_point *p0, t_point *p1)
{
	int		i;
	uint8_t	tile;

	tile = rand() % 12 + 1;
	tile = tile > 6 ? tile + 30 : tile;
	i = -1;
	if (!check_space(v, p0, p1))
		return (0);
	while (++i < p1->r - p0->r)
	{
		v->map[p0->r + i][p0->c] = tile;
		v->map[p0->r + i][p1->c] = tile;
		v->map[p0->r + i][p0->c + 1] = C(r) ? v->map[p0->r + i][p0->c + 1] : -1;
		v->map[p0->r + i][p1->c - 1] = C(r) ? v->map[p0->r + i][p1->c - 1] : -1;
	}
	i = -1;
	while (++i < p1->c - p0->c)
	{
		v->map[p0->r][p0->c + i] = tile;
		v->map[p1->r][p0->c + i] = tile;
		v->map[p0->r + 1][p0->c + i] = C(c) ? v->map[p0->r + 1][p0->c + i] : -1;
		v->map[p1->r - 1][p0->c + i] = C(c) ? v->map[p1->r - 1][p0->c + i] : -1;
	}
	return (1);
}

static int		find_dir(t_view *v, t_point *loc)
{
	int		dir;

	dir = 0;
	if (v->map[loc->r + 1][loc->c] == -1)
		dir += 1;
	if (v->map[loc->r][loc->c - 1] == -1)
		dir += 2;
	if (v->map[loc->r - 1][loc->c] == -1)
		dir += 4;
	if (v->map[loc->r][loc->c + 1] == -1)
		dir += 8;
	if (dir == 1)
		return (0);
	else if (dir == 2)
		return (1);
	else if (dir == 4)
		return (2);
	else if (dir == 8)
		return (3);
	else
		return (-1);
}

static void		double_free(t_point *p0, t_point *p1)
{
	free(p0);
	free(p1);
}

static int		doorify(t_view *v, t_point *l, int out, int dir)
{
	if (out)
	{
		if (dir == 0)
			v->map[l->r - 1][l->c] = 0;
		else if (dir == 1)
			v->map[l->r][l->c + 1] = 0;
		else if (dir == 2)
			v->map[l->r + 1][l->c] = 0;
		else if (dir == 3)
			v->map[l->r][l->c - 1] = 0;
		v->map[l->r][l->c] = 0;
	}
	return	(out);
}

static int		gen_room(t_view *v, t_point	*l)
{
	int		dir;
	int		size[2];
	int		out;
	t_point	*p0;
	t_point	*p1;

	dir = find_dir(v, l);
	size[0] = rand() % (ROOM_MAX - ROOM_MIN) + ROOM_MIN;
	size[1] = rand() % (ROOM_MAX - ROOM_MIN) + ROOM_MIN;
	if (dir == 0)
		out = make_room(v, p0 = get_point(l->r - size[0] - 1, l->c - size[1] / 2),
			p1 = get_point(l->r - 1, l->c + size[1] / 2));
	else if (dir == 1)
		out = make_room(v, p0 = get_point(l->r - size[0] / 2, l->c + 1),
			p1 = get_point(l->r + size[0] / 2, l->c + size[1] + 1));
	else if (dir == 2)
		out = make_room(v, p0 = get_point(l->r + 1, l->c - size[1] / 2),
			p1 = get_point(l->r + size[0] + 1, l->c + size[1] / 2));
	else if (dir == 3)
		out = make_room(v, p0 = get_point(l->r - size[0] / 2, l->c - size[1] - 1),
			p1 = get_point(l->r + size[0] / 2, l->c - 1));
	else
		return (0);
	double_free(p0, p1);
	return (doorify(v, l, out, dir));
}

static void		make_starting_room(t_view *v)
{
	int		i;

	v->map[MAP_HEIGHT / 2 - 3][MAP_WIDTH / 2 - 3] = 1;
	v->map[MAP_HEIGHT / 2 + 3][MAP_WIDTH / 2 - 3] = 1;
	v->map[MAP_HEIGHT / 2 - 3][MAP_WIDTH / 2 + 3] = 1;
	v->map[MAP_HEIGHT / 2 + 3][MAP_WIDTH / 2 + 3] = 1;
	i = MAP_HEIGHT / 2 - 3;
	while (++i <= MAP_HEIGHT / 2 + 2)
	{
		v->map[i][MAP_WIDTH / 2 - 3] = 1;
		v->map[i][MAP_WIDTH / 2 + 3] = 1;
		v->map[i][MAP_WIDTH / 2 - 2] = -1;
		v->map[i][MAP_WIDTH / 2 + 2] = -1;
	}
	i = MAP_WIDTH / 2 - 3;
	while (++i <= MAP_WIDTH / 2 + 2)
	{
		v->map[MAP_HEIGHT / 2 - 3][i] = 1;
		v->map[MAP_HEIGHT / 2 + 3][i] = 1;
		v->map[MAP_HEIGHT / 2 - 2][i] = -1;
		v->map[MAP_HEIGHT / 2 + 2][i] = -1;
	}
}

static void		get_ready(t_view *v)
{
	int	y;
	int	x;

	y = -1;
	v->tab = (double*)ft_memalloc(sizeof(double) * WIN_HEIGHT);
	v->darkness = (double*)ft_memalloc(sizeof(double) * WIN_HEIGHT);
	v->map = (char**)ft_memalloc(sizeof(char*) * MAP_HEIGHT);
	while (++y < MAP_HEIGHT)
	{
		x = -1;
		v->map[y] = (char*)ft_memalloc(sizeof(char) * MAP_WIDTH);
		while (++x < MAP_WIDTH)
			if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1)
				v->map[y][x] = 45;
	}
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		v->tab[y] = WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
		v->darkness[y] = MAX(0, MIN(1, (double)y / (double)WIN_HEIGHT));
	}
	v->pressed = (t_keys*)ft_memalloc(sizeof(t_keys));
	v->cur_time = 0;
	make_starting_room(v);
	srand(time(NULL));
}

static void		normalize(t_view *v)
{
	int		r;
	int		c;

	r = -1;
	while (++r < MAP_HEIGHT)
	{
		c = -1;
		while (++c < MAP_WIDTH)
			if (v->map[r][c] < 0)
				v->map[r][c] = 0;
	}
}

static void		map_error(void)
{
	ft_putendl("MAP ERROR!");
	ft_putendl("You are probably not leaving enough space for all the rooms!");
	exit(0);
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
		if (view->map[r][c] <= 0)
			continue ;
		p = get_point(r, c);
		if (gen_room(view, p))
			built_num++;
		free(p);
		stopper[1] = stopper[0] == built_num ? stopper[1] + 1 : 0;
	}
	if (stopper[1] >= 100)
		map_error();
	normalize(view);
}
