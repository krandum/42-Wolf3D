/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgfx.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/10 17:01:40 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/10 17:01:41 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGFX_H
# define LIBGFX_H

# include "libft.h"
# include "mlx.h"
# include "math.h"

# define WIN_WIDTH 1600
# define WIN_HEIGHT 1200

# define T_SIZE 64

# define W_W WIN_WIDTH
# define W_H WIN_HEIGHT

typedef int		t_color;

typedef struct	s_2dp
{
	float	x;
	float	y;
}				t_2dp;

typedef struct	s_player
{
	t_2dp		*pos;
	t_2dp		*dir;
	t_2dp		*cam;
}				t_player;

typedef struct	s_view
{
	void		*id;
	void		*win;
	void		*img;
	t_player	*player;
	char		*pixels;
	t_color		**textures;
	t_color		*colors;
	char		**map;
	double		y_shift;
	int			num_colors;
	int			width;
	int			height;
	int			bits_per_pixel;
	int			size_line;
	int			endian;
}				t_view;

void		ft_init_color_table(t_view *view, int colors);
t_2dp		*ft_get_2d_point(float x, float y);
void		ft_color_pixel(t_view *v, int x, int y, int iter);

#endif
