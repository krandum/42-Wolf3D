/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/24 09:58:59 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/24 09:59:00 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_point			*get_point(int r, int c)
{
	t_point	*out;

	out = (t_point*)ft_memalloc(sizeof(t_point));
	out->r = r;
	out->c = c;
	return (out);
}

int				check_space(t_view *v, t_point *p0, t_point *p1)
{
	int		r;
	int		c;

	if (p0->r >= MAP_HEIGHT - 1 || p1->r >= MAP_HEIGHT - 1 || p1->r <= 1
		|| p0->r <= 1)
		return (0);
	if (p0->c >= MAP_WIDTH - 1 || p1->c >= MAP_WIDTH - 1 || p1->c <= 1
		|| p0->c <= 1)
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

void			map_error(void)
{
	ft_putendl("MAP ERROR!");
	ft_putendl("You are probably not leaving enough space for all the rooms!");
	exit(0);
}

int				find_dir(t_view *v, t_point *loc)
{
	int		dir;

	dir = 0;
	if (v->map[loc->r + 1][loc->c] > 0)
		dir += 1;
	if (v->map[loc->r][loc->c - 1] > 0)
		dir += 2;
	if (v->map[loc->r - 1][loc->c] > 0)
		dir += 4;
	if (v->map[loc->r][loc->c + 1] > 0)
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
