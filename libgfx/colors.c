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

	out = (t_color*)malloc(sizeof(t_color) * T_SIZE * T_SIZE);
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

t_2dp		*ft_get_2d_point(float x, float y)
{
	t_2dp	*out;

	out = (t_2dp*)malloc(sizeof(t_2dp));
	out->x = x;
	out->y = y;
	return (out);
}
