/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rooms.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 09:52:57 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/24 09:52:58 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

#define C(x) (i == 0 || i == p1->x - p0->x)

static int	doorify(t_view *v, t_point *l, int out)
{
	if (out)
		v->map[l->r][l->c] = -1;
	return (out);
}

static int	make_room(t_view *v, t_point *p0, t_point *p1)
{
	int		x;
	int		y;
	uint8_t	tile;

	tile = rand() % 7 + 1;
	if (!check_space(v, p0, p1))
		return (0);
	y = p0->r;
	while (++y < p1->r)
	{
		x = p0->c;
		while (++x < p1->c)
			v->map[y][x] = tile;
	}
	return (1);
}

#define ASSIGN(pt,y,x) (pt = get_point(l->r + y, l->c + x))

int			gen_room(t_view *v, t_point *l)
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
		out = make_room(v, ASSIGN(p0, -size[0] - 1, -size[1] / 2),
			ASSIGN(p1, 0, size[1] / 2));
	else if (dir == 1)
		out = make_room(v, ASSIGN(p0, -size[0] / 2, 0),
			ASSIGN(p1, size[0] / 2, size[1] + 1));
	else if (dir == 2)
		out = make_room(v, ASSIGN(p0, 0, -size[1] / 2),
			ASSIGN(p1, size[0] + 1, size[1] / 2));
	else if (dir == 3)
		out = make_room(v, ASSIGN(p0, -size[0] / 2, -size[1] - 1),
			ASSIGN(p1, size[0] / 2, 0));
	else
		return (0);
	double_free(p0, p1);
	return (doorify(v, l, out));
}

int			gen_corr(t_view *v, t_point *l)
{
	int		dir;
	int		size;
	int		out;
	t_point	*p0;
	t_point	*p1;

	dir = find_dir(v, l);
	size = rand() % (CORR_MAX - CORR_MIN) + CORR_MIN + 1;
	if (dir == 0)
		out = make_room(v, ASSIGN(p0, -size, -1),
			ASSIGN(p1, 0, 1));
	else if (dir == 1)
		out = make_room(v, ASSIGN(p0, -1, 0),
			ASSIGN(p1, 1, size));
	else if (dir == 2)
		out = make_room(v, ASSIGN(p0, 0, -1),
			ASSIGN(p1, size, 1));
	else if (dir == 3)
		out = make_room(v, ASSIGN(p0, -1, -size),
			ASSIGN(p1, 1, 0));
	else
		return (0);
	double_free(p0, p1);
	return (doorify(v, l, out));
}

void		make_starting_room(t_view *v)
{
	int		x;
	int		y;

	y = MAP_HEIGHT / 2 - 4;
	while (++y < MAP_HEIGHT / 2 + 4)
	{
		x = MAP_WIDTH / 2 - 4;
		while (++x < MAP_WIDTH / 2 + 4)
			v->map[y][x] = 1;
	}
}
