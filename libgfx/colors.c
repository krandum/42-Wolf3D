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

t_color		ft_color_mult(t_color c, float r)
{
	t_byte	rgb[3];
	t_color	out;

	rgb[0] = (t_byte)((c & 0xFF0000) >> 16);
	rgb[1] = (t_byte)((c & 0xFF00) >> 8);
	rgb[2] = (t_byte)(c & 0xFF);
	rgb[0] = MAX(0, MIN(0xFF, rgb[0] * r));
	rgb[1] = MAX(0, MIN(0xFF, rgb[1] * r));
	rgb[2] = MAX(0, MIN(0xFF, rgb[2] * r));
	out = ((t_color)rgb[0]) << 16 | ((t_color)rgb[1]) << 8 | (t_color)rgb[2];
	return (out);
}

/*
**	Not portable, numbers are specific to the sprite sheet.
*/

t_color		*ft_get_texture(t_view *v, int offset)
{
	t_color	*out;
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	int		i;

	out = (t_color*)malloc(sizeof(t_color) * (T_SIZE * T_SIZE));
	i = -1;
	while (++i < T_SIZE * T_SIZE)
	{
		if (i % 32 == 0 && i / 32 > 0)
			offset += 256 * 3;
		blue = v->texture_data[offset++];
		green = v->texture_data[offset++];
		red = v->texture_data[offset++];
		out[i] = ((int)red) << 16 | ((int)green) << 8 | blue;
	}
	return (out);
}

t_color		*ft_get_fist(t_view *v, int number)
{
	t_color	*out;
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	int		i;
	int		offset;

	out = (t_color*)malloc(sizeof(t_color) * 128 * 256);
	i = -1;
	offset = number * 128 * 256 * 3;
	while (++i < 128 * 256)
	{
		blue = v->texture_data[offset++];
		green = v->texture_data[offset++];
		red = v->texture_data[offset++];
		out[i] = ((int)red) << 16 | ((int)green) << 8 | blue;
	}
	return (out);
}

t_2dp		*ft_get_2d_point(float x, float y)
{
	t_2dp	*out;

	out = (t_2dp*)malloc(sizeof(t_2dp));
	out->x = x;
	out->y = y;
	return (out);
}
