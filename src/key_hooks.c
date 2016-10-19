/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/16 21:00:01 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/16 21:00:02 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

#define P_PX view->player->pos->x
#define P_PY view->player->pos->y
#define P_DX view->player->dir->x
#define P_DY view->player->dir->y
#define P_CX view->player->cam->x
#define P_CY view->player->cam->y

#define MAP(x, y) view->map[x][y]

#define M_S (view->player->mov_speed)
#define R_S (view->player->rot_speed)

static void		key_hook_walk(t_view *view)
{
	double	temp;
	int		sign;

	if (view->pressed->key_w)
	{
		P_PX += view->map[(int)(P_PX + P_DX * M_S)][(int)P_PY] ? 0 : P_DX * M_S;
		P_PY += view->map[(int)P_PX][(int)(P_PY + P_DY * M_S)] ? 0 : P_DY * M_S;
	}
	else if (view->pressed->key_s)
	{
		P_PX -= view->map[(int)(P_PX - P_DX * M_S)][(int)P_PY] ? 0 : P_DX * M_S;
		P_PY -= view->map[(int)P_PX][(int)(P_PY - P_DY * M_S)] ? 0 : P_DY * M_S;
	}
	if (view->pressed->key_a || view->pressed->key_d)
	{
		sign = view->pressed->key_d ? -1 : 1;
		temp = P_DX;
		P_DX = P_DX * cos(sign * R_S) - P_DY * sin(sign * R_S);
		P_DY = temp * sin(sign * R_S) + P_DY * cos(sign * R_S);
		temp = P_CX;
		P_CX = P_CX * cos(sign * R_S) - P_CY * sin(sign * R_S);
		P_CY = temp * sin(sign * R_S) + P_CY * cos(sign * R_S);
	}
}

static void		key_hook_strafe(t_view *view)
{
	double	tx;
	double	ty;

	if (view->pressed->key_q)
	{
		tx = P_DX * cos(M_PI / 2.0) - P_DY * sin(M_PI / 2.0);
		ty = P_DX * sin(M_PI / 2.0) + P_DY * cos(M_PI / 2.0);
		P_PX += view->map[(int)(P_PX + tx * M_S)][(int)P_PY] ? 0 : tx * M_S;
		P_PY += view->map[(int)P_PX][(int)(P_PY + ty * M_S)] ? 0 : ty * M_S;
	}
	else if (view->pressed->key_e)
	{
		tx = P_DX * cos(-M_PI / 2.0) - P_DY * sin(-M_PI / 2.0);
		ty = P_DX * sin(-M_PI / 2.0) + P_DY * cos(-M_PI / 2.0);
		P_PX += view->map[(int)(P_PX + tx * M_S)][(int)P_PY] ? 0 : tx * M_S;
		P_PY += view->map[(int)P_PX][(int)(P_PY + ty * M_S)] ? 0 : ty * M_S;
	}
}

int				key_pressed_hook(int keycode, t_view *view)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(view->id, view->win);
		exit(0);
	}
	if (keycode == KEY_FRONT || keycode == KEY_UP)
		view->pressed->key_w = 1;
	else if (keycode == KEY_BACK || keycode == KEY_DOWN)
		view->pressed->key_s = 1;
	else if (keycode == KEY_TURN_L || keycode == KEY_LEFT)
		view->pressed->key_a = 1;
	else if (keycode == KEY_TURN_R || keycode == KEY_RIGHT)
		view->pressed->key_d = 1;
	else if (keycode == KEY_STRAFE_L)
		view->pressed->key_q = 1;
	else if (keycode == KEY_STRAFE_R)
		view->pressed->key_e = 1;
	return (0);
}

int				key_released_hook(int keycode, t_view *view)
{
	if (keycode == KEY_FRONT || keycode == KEY_UP)
		view->pressed->key_w = 0;
	else if (keycode == KEY_BACK || keycode == KEY_DOWN)
		view->pressed->key_s = 0;
	else if (keycode == KEY_TURN_L || keycode == KEY_LEFT)
		view->pressed->key_a = 0;
	else if (keycode == KEY_TURN_R || keycode == KEY_RIGHT)
		view->pressed->key_d = 0;
	else if (keycode == KEY_STRAFE_L)
		view->pressed->key_q = 0;
	else if (keycode == KEY_STRAFE_R)
		view->pressed->key_e = 0;
	return (0);
}

int				generic_hook(t_view *view)
{
	double	frame_time;

	view->old_time = view->cur_time;
	view->cur_time = clock();
	frame_time = (view->cur_time - view->old_time) / 1000000.0;
	view->player->mov_speed = frame_time * 5.0;
	view->player->rot_speed = M_PI * frame_time;
	key_hook_walk(view);
	key_hook_strafe(view);
	draw_reload(view);
	return (0);
}