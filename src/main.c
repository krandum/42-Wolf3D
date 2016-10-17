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

char	*world_map[24] =
{
 	"\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\4\7\7\7\7\7\7\7\7",
 	"\4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7\0\0\0\0\0\0\7",
 	"\4\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7",
 	"\4\0\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7",
 	"\4\0\3\0\0\0\0\0\0\0\0\0\0\0\0\0\7\0\0\0\0\0\0\7",
 	"\4\0\4\0\0\0\0\5\5\5\5\5\5\5\5\5\7\7\0\7\7\7\7\7",
 	"\4\0\5\0\0\0\0\5\0\5\0\5\0\5\0\5\7\0\0\0\7\7\7\1",
 	"\4\0\6\0\0\0\0\5\0\0\0\0\0\0\0\5\7\0\0\0\0\0\0\1",
 	"\4\0\7\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7\7\7\1",
 	"\4\0\1\0\0\0\0\5\0\0\0\0\0\0\0\5\7\0\0\0\0\0\0\1",
 	"\4\0\0\0\0\0\0\5\0\0\0\0\0\0\0\5\7\0\0\0\7\7\7\1",
 	"\4\0\0\0\0\0\0\5\5\5\5\0\5\5\5\5\7\7\7\7\7\7\7\1",
 	"\6\6\6\6\6\6\6\6\6\6\6\0\6\6\6\6\6\6\6\6\6\6\6\6",
 	"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4",
 	"\6\6\6\6\6\6\0\6\6\6\6\0\6\6\6\6\6\6\6\6\6\6\6\6",
 	"\4\4\4\4\4\4\0\4\4\4\6\0\6\2\2\2\2\2\2\2\3\3\3\3",
 	"\4\0\0\0\0\0\0\0\0\4\6\0\6\2\0\0\0\0\0\2\0\0\0\2",
 	"\4\0\0\0\0\0\0\0\0\0\0\0\6\2\0\0\5\0\0\2\0\0\0\2",
 	"\4\0\0\0\0\0\0\0\0\4\6\0\6\2\0\0\0\0\0\2\2\0\2\2",
 	"\4\0\6\0\6\0\0\0\0\4\6\0\0\0\0\0\5\0\0\0\0\0\0\2",
 	"\4\0\0\5\0\0\0\0\0\4\6\0\6\2\0\0\0\0\0\2\2\0\2\2",
 	"\4\0\6\0\6\0\0\0\0\4\6\0\6\2\0\0\5\0\0\2\0\0\0\2",
 	"\4\0\0\0\0\0\0\0\0\4\6\0\6\2\0\0\0\0\0\2\0\0\0\2",
 	"\4\4\4\4\4\4\4\4\4\4\1\1\1\2\2\2\2\2\2\3\3\3\3\3"
};

#define TEXT(z) (v->textures[z][T_SIZE * y + x])

static void		gen_textures(t_view *v)
{
	int		x;
	int		y;
	t_color	xor_c;
	t_color	y_c;
	t_color	xy_c;

	v->textures = (t_color**)malloc(sizeof(t_color*) * 7);
	y = -1;
	while (++y < 7)
		v->textures[y] = (t_color*)malloc(sizeof(t_color) * T_SIZE * T_SIZE);
	x = -1;
	while (++x < T_SIZE && (y = -1) == -1)
		while (++y < T_SIZE)
		{
			xor_c = (x * 0x100 / T_SIZE) ^ (y * 0x100 / T_SIZE);
			y_c = y * 0x100 / T_SIZE;
			xy_c = y * 0x80 / T_SIZE + x * 0x80 / T_SIZE;
			TEXT(0) = 0x10000 * 0xFE * (x != y && x != T_SIZE - y);
			TEXT(1) = xy_c + 0x100 * xy_c + 0x10000 * xy_c;
			TEXT(2) = 0x100 * xy_c + 0x10000 * xy_c;
			TEXT(3) = xor_c + 0x100 * xor_c + 0x10000 * xor_c;
			TEXT(4) = 0x100 * xor_c;
			TEXT(5) = 0x10000 * 192 * (x % 16 && y % 16);
			TEXT(6) = 0x10000 * y_c;
		}
}

int				main(void)
{
	t_view	*view;

	view = (t_view*)malloc(sizeof(t_view));
	printf("LOADING...\n");
	view->player = (t_player*)malloc(sizeof(t_player));
	view->player->pos = ft_get_2d_point(21.5, 11.5);
	view->player->dir = ft_get_2d_point(-1.0, 0.0);
	view->player->cam = ft_get_2d_point(0.0, 0.6666666);
	printf("\t-Player coordinates set...\n");
	view->id = mlx_init();
	view->win = mlx_new_window(view->id, WIN_WIDTH, WIN_HEIGHT, "42-Wolf 3D");
	view->map = world_map;
	view->num_colors = 8;
	printf("\t-Window created...\n");
	ft_init_color_table(view, 9);
	printf("\t-Color table initialized...\n");
	gen_textures(view);
	printf("\t-Placeholder textures set...\n");
	mlx_expose_hook(view->win, expose_hook, view);
	printf("\t-Window exposed...\n");
	mlx_hook(view->win, 2, 3, key_hook, view);
	printf("\t-Keyboard hooked...\n");
	mlx_loop(view->id);
}
