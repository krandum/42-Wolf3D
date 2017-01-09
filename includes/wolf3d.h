/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/16 17:18:58 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/16 17:18:58 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include "mlx.h"
# include "libgfx.h"
# include <fcntl.h>
# include <sys/time.h>

# define KEY_ESC 53

# define KEY_FRONT 13
# define KEY_BACK 1
# define KEY_TURN_L 0
# define KEY_TURN_R 2
# define KEY_STRAFE_L 12
# define KEY_STRAFE_R 14

# define KEY_RUN 257

# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124

# define MAP_WIDTH 100
# define MAP_HEIGHT 100

# define BUILD_NUM 60
# define ACCESS_NUM 20
# define ROOM_DEN 65
# define ROOM_MIN 7
# define ROOM_MAX 14
# define CORR_MIN 6
# define CORR_MAX 12

typedef struct	s_point
{
	int	r;
	int	c;
}				t_point;

void			draw_reload(t_view *view);
void			render_column(t_view *v, t_render *r);
void			iter(t_view *v, t_render *r);
void			init_points(t_view *v, t_render *r);
void			draw_floor(t_view *v, t_render *r, t_2dp *floor_w,
	double weight);
int				expose_hook(t_view *view);
int				exit_hook(t_view *view);
int				key_pressed_hook(int keycode, t_view *view);
int				key_released_hook(int keycode, t_view *view);
int				generic_hook(t_view *view);
void			gen_dungeon(t_view *view);
void			make_starting_room(t_view *v);
int				gen_room(t_view *v, t_point *l);
int				gen_corr(t_view *v, t_point *l);
int				check_space(t_view *v, t_point *p0, t_point *p1);
int				find_dir(t_view *v, t_point *loc);
t_point			*get_point(int r, int c);
void			map_error(void);
void			double_free(t_point *p0, t_point *p1);

#endif
