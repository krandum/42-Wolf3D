/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/15 15:05:12 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/15 15:05:14 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgfx.h"

#define MUCHIEST(x, y, z) (x > y ? (x > z ? x : z) : (y > z ? y : z))

void		ft_color_pixel(t_view *v, int x, int y, int iter)
{
	int				i;
	int				color;
	int				rgb[3];

	if (iter >= 0)
		color = v->colors[iter % v->num_colors];
	else
	{
		iter *= -1;
		color = (int)(v->colors[iter % v->num_colors]);
		rgb[0] = color & 0xFF0000;
		rgb[1] = color & 0xFF00;
		rgb[2] = color & 0xFF;
		color -= (int)(MUCHIEST(rgb[0], rgb[1], rgb[2]) / 0.1);
	}
	i = (x * 4) + (y * v->size_line);
	v->pixels[i] = color;
	v->pixels[++i] = color >> 8;
	v->pixels[++i] = color >> 16;
}

void		ft_init_color_table(t_view *view, int colors)
{
	int				i;
	float			f;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	view->colors = (t_color*)malloc(sizeof(t_color) * colors);
	f = 0;
	i = -1;
	while (++i < colors)
	{
		r = (unsigned char)((-cos(f) + 1.0) * 127.0);
		g = (unsigned char)((sin(f) + 1.0) * 127.0);
		b = (unsigned char)((cos(f) + 1.0) * 127.0);
		view->colors[i] = ((int)r) << 16 | ((int)g) << 8 | b;
		f += 2 * M_PI / (colors + 1);
	}
}

t_2dp		*ft_get_2d_point(float x, float y)
{
	t_2dp	*out;

	out = (t_2dp*)malloc(sizeof(t_2dp));
	out->x = x;
	out->y = y;
	return (out);
}
