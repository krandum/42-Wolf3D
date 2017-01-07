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

static int	doorify(t_view *v, t_point *l, int out, int dir)
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
	return (out);
}

static int	make_room(t_view *v, t_point *p0, t_point *p1)
{
	int		i;
	uint8_t	tile;

	tile = rand() % 4 + 1;
	tile = (tile % 2 == 0 ? 4 : 1) + (tile > 2 ? 36 : 0);
	i = -1;
	if (!check_space(v, p0, p1))
		return (0);
	while (++i <= p1->r - p0->r)
	{
		v->map[p0->r + i][p0->c] = tile;
		v->map[p0->r + i][p1->c] = tile;
		v->map[p0->r + i][p0->c + 1] = C(r) ? v->map[p0->r + i][p0->c + 1] : -1;
		v->map[p0->r + i][p1->c - 1] = C(r) ? v->map[p0->r + i][p1->c - 1] : -1;
	}
	i = -1;
	while (++i <= p1->c - p0->c)
	{
		v->map[p0->r][p0->c + i] = tile;
		v->map[p1->r][p0->c + i] = tile;
		v->map[p0->r + 1][p0->c + i] = C(c) ? v->map[p0->r + 1][p0->c + i] : -1;
		v->map[p1->r - 1][p0->c + i] = C(c) ? v->map[p1->r - 1][p0->c + i] : -1;
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
			ASSIGN(p1, -1, size[1] / 2));
	else if (dir == 1)
		out = make_room(v, ASSIGN(p0, -size[0] / 2, 1),
			ASSIGN(p1, size[0] / 2, size[1] + 1));
	else if (dir == 2)
		out = make_room(v, ASSIGN(p0, 1, -size[1] / 2),
			ASSIGN(p1, size[0] + 1, size[1] / 2));
	else if (dir == 3)
		out = make_room(v, ASSIGN(p0, -size[0] / 2, -size[1] - 1),
			ASSIGN(p1, size[0] / 2, -1));
	else
		return (0);
	double_free(p0, p1);
	return (doorify(v, l, out, dir));
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
			ASSIGN(p1, -1, 1));
	else if (dir == 1)
		out = make_room(v, ASSIGN(p0, -1, 1),
			ASSIGN(p1, 1, size));
	else if (dir == 2)
		out = make_room(v, ASSIGN(p0, 1, -1),
			ASSIGN(p1, size, 1));
	else if (dir == 3)
		out = make_room(v, ASSIGN(p0, -1, -size),
			ASSIGN(p1, 1, -1));
	else
		return (0);
	double_free(p0, p1);
	return (doorify(v, l, out, dir));
}

void		make_starting_room(t_view *v)
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
