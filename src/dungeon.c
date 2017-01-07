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
		v->tab[y] = 1.5 * WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
		v->darkness[y] = MAX(0, MIN(1, (double)y / (double)WIN_HEIGHT) + 0.15);
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

static int		gen_something(t_view *view, t_point *p)
{
	int		picker;

	picker = rand() % 100 + 1;
	if (picker < BUILD_NUM)
		return (gen_room(view, p));
	else
		return (gen_corr(view, p));
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
		if (gen_something(view, p))
			built_num++;
		free(p);
		stopper[1] = stopper[0] == built_num ? stopper[1] + 1 : 0;
	}
	if (stopper[1] >= 100)
		map_error();
	normalize(view);
}
