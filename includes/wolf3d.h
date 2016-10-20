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

# ifdef __MACH__
#  include <mach/clock.h>
#  include <mach/mach.h>
# endif


# define KEY_ESC 53

# define KEY_FRONT 13 //W
# define KEY_BACK 1 //S
# define KEY_TURN_L 0 //A
# define KEY_TURN_R 2 //D
# define KEY_STRAFE_L 12 //Q
# define KEY_STRAFE_R 14 //E

# define KEY_RUN 257 //SHIFT

# define KEY_UP 126 //UP
# define KEY_DOWN 125 //DOWN
# define KEY_LEFT 123 //LEFT
# define KEY_RIGHT 124 //RIGHT

# define MAP_WIDTH 51
# define MAP_HEIGHT 51

void			draw_reload(t_view *view);
void			render_column(t_view *v, int x);
int				expose_hook(t_view *view);
int				key_pressed_hook(int keycode, t_view *view);
int				key_released_hook(int keycode, t_view *view);
int				generic_hook(t_view *view);
void			gen_dungeon(t_view *view);


#endif
