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
# include "time.h"
# include <pthread.h>
# include <stdio.h>

# ifdef __MACH__
#  include <mach/clock.h>
#  include <mach/mach.h>
# endif

# define WIN_WIDTH 1600
# define WIN_HEIGHT 1200

# define T_SIZE 32
# define T_FILE "textures/sheet.bmp"

# define W_W WIN_WIDTH
# define W_H WIN_HEIGHT

typedef int			t_color;

typedef struct		s_2dp
{
	float			x;
	float			y;
}					t_2dp;

typedef struct		s_3dp
{
	float			x;
	float			y;
	float			z;
}					t_3dp;

typedef struct		s_keys
{
	uint8_t			key_w:1;
	uint8_t			key_a:1;
	uint8_t			key_s:1;
	uint8_t			key_d:1;
	uint8_t			key_q:1;
	uint8_t			key_e:1;
	uint8_t			key_sh:1;
	uint8_t			running:1;
}					t_keys;

typedef struct		s_player
{
	t_2dp			*pos;
	t_2dp			*dir;
	t_2dp			*cam;
	double			mov_speed;
	double			rot_speed;
}					t_player;

typedef struct		s_view
{
	void			*id;
	void			*win;
	void			*img;
	t_player		*player;
	t_keys			*pressed;
	char			*pixels;
	unsigned char	*texture_data;
	t_color			**textures;
	t_color			*colors;
	double			*tab;
	double			*darkness;
	char			**map;
	int				width;
	int				height;
	int				num_colors;
	int				bits_per_pixel;
	int				s_line;
	int				endian;
	long			cur_time;
	long			old_time;
}					t_view;

typedef struct		s_render
{
	double			rayposx;
	double			rayposy;
	double			raydx;
	double			raydy;
	int				mapx;
	int				mapy;
	double			sidedistx;
	double			sidedisty;
	double			deltadistx;
	double			deltadisty;
	int				stepx;
	int				stepy;
	int				x;
	int				y;
	double			walldist;
	int				lineheight;
	int				drawstart;
	int				drawend;
	int				hit;
	int				side;
	double			camx;
	int				texnum;
	double			wallx;
	int				texx;
	int				texy;
	float			red;
	float			green;
	float			blue;
	t_color			color;
}					t_render;

typedef struct		s_split
{
	int				x_start;
	int				x_end;
	t_view			*view;
	t_render		*render;
	pthread_t		thread;
}					t_split;

t_color				*ft_get_texture(t_view *v, int offset);
void				ft_get_time(struct timespec *ts);
t_2dp				*ft_get_2d_point(float x, float y);
void				ft_color_pixel(t_view *v, int x, int y, int iter);

#endif
