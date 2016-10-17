/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/16 18:32:04 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/16 18:32:05 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgfx.h"

t_2dp		*ft_rotate_vector(t_2dp vec, float theta)
{
	t_2dp	*out;

	out = ft_get_2d_point(cos(theta) * vec->x - sin(theta) * vec->y,
		sin(theta) * vec->x + cos(theta) * vec->y);
	return (out);
}
