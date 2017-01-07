/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/16 18:40:01 by palatorr          #+#    #+#             */
/*   Updated: 2016/10/16 18:40:01 by palatorr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Doesn't actually work for any given sprite sheet right now.
*/

static void		gen_textures(t_view *v)
{
	FILE			*f;
	int				size;
	unsigned char	info[54];
	int				i;

	f = fopen(T_FILE, "rb");
	fread(info, sizeof(unsigned char), 54, f);
	v->width = *(int*)&info[18];
	v->height = *(int*)&info[22] * -1;
	size = 3 * v->width * v->height;
	v->texture_data = (unsigned char*)malloc(size);
	fread(v->texture_data, 1, size, f);
	fclose(f);
	v->textures = (t_color**)malloc(sizeof(t_color*) * 90);
	i = -1;
	while (++i < 90)
		v->textures[i] = ft_get_texture(v, ((i / 9) * 0x6C00) + (i % 9) * 0x60);
}

int				exit_hook(t_view *view)
{
	mlx_destroy_window(view->id, view->win);
	exit(0);
}

int				expose_hook(t_view *view)
{
	draw_reload(view);
	return (0);
}

void			double_free(t_point *p0, t_point *p1)
{
	free(p0);
	free(p1);
}

int				main(void)
{
	t_view	*view;

	view = (t_view*)malloc(sizeof(t_view));
	printf("LOADING...\n");
	view->player = (t_player*)malloc(sizeof(t_player));
	view->player->pos = ft_get_2d_point(MAP_HEIGHT / 2 + 0.01,
		MAP_WIDTH / 2 + 0.01);
	view->player->dir = ft_get_2d_point(1.0, 0.0);
	view->player->cam = ft_get_2d_point(0.0, -0.6666666);
	printf("\t-Player coordinates set...\n");
	view->id = mlx_init();
	view->win = mlx_new_window(view->id, WIN_WIDTH, WIN_HEIGHT, "42-Wolf 3D");
	gen_textures(view);
	printf("\t-Textures initialized...\n");
	gen_dungeon(view);
	printf("\t-Map generated...\n");
	mlx_expose_hook(view->win, expose_hook, view);
	mlx_hook(view->win, 2, 0, key_pressed_hook, view);
	mlx_hook(view->win, 3, 0, key_released_hook, view);
	mlx_do_key_autorepeatoff(view->id);
	mlx_loop_hook(view->id, generic_hook, view);
	printf("\t-Hooks in place...\nRUNNING...\n");
	mlx_hook(view->win, 17, 0, exit_hook, view);
	mlx_loop(view->id);
}
