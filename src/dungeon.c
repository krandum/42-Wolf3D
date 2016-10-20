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

static void	get_ready(t_view *v)
{
	int	y;
	int	x;

	y = -1;
	v->tab = (double*)ft_memalloc(sizeof(double) * WIN_HEIGHT);
	while (++y < MAP_HEIGHT)
	{
		x = -1;
		v->map[y] = ft_strdup(v->map[y]);
		while (++x < MAP_WIDTH)
			v->map[y][x] -= '0';
	}
	y = -1;
	while (++y < WIN_HEIGHT)
		v->tab[y] = WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
	v->pressed = (t_keys*)ft_memalloc(sizeof(t_keys));
	v->cur_time = 0;
	v->old_time = 0;
}

void		gen_dungeon(t_view *view)
{
	get_ready(view);
}
