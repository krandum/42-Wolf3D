# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: palatorr <palatorr@student.42.us.or>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/16 17:08:52 by palatorr          #+#    #+#              #
#    Updated: 2016/10/16 17:08:53 by palatorr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= wolf3d

SRC		= main.c render.c hooks.c dungeon.c
OBJ		= $(addprefix $(OBJDIR),$(SRC:.c=.o))

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -O3

LIBFT	= ./libft/libft.a
FTINC	= -I ./libft/
FTLINK	= -L ./libft/ -lft

LIBGFX	= ./libgfx/libgfx.a
GFXINC	= -I ./libgfx/
GFXLINK	= -L ./libgfx/ -lgfx

LIBMLX	= ./minilibx/libmlx.a
MLXINC	= -I ./minilibx/
MLXLINK	= -L ./minilibx/ -lmlx -framework OpenGL -framework AppKit

SRCDIR	= ./src/
INCDIR	= ./includes/
OBJDIR	= ./obj/

.PHONY: full libft libmlx libgfx regfx re clean rclean fclean rfclean

all: obj $(NAME)

full: obj libft libgfx libmlx $(NAME)

obj:
	mkdir $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) $(GFXINC) $(MLXINC) -I $(INCDIR) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(FTLINK) $(GFXLINK) $(MLXLINK) -o $(NAME)

libft:
	make -C ./libft re

libgfx:
	make -C ./libgfx re

libmlx:
	make -C ./minilibx re

clean:
	rm -rf $(OBJDIR)

rclean:
	rm -rf $(OBJDIR)
	make -C ./libft clean
	make -C ./libgfx clean
	make -C ./minilibx clean

fclean: clean
	rm -rf $(NAME)

rfclean: rclean
	rm -rf $(NAME)
	make -C ./libft fclean
	make -C ./libgfx fclean

re: fclean all

f: gfx re
	./$(NAME)

regfx: gfx fclean all

fullre: rfclean full
