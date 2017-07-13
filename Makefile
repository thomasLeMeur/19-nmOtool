# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 18:11:41 by tle-meur          #+#    #+#              #
#    Updated: 2017/02/24 23:16:11 by tle-meur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1	=	ft_nm
NAME2	=	ft_otool

CC		=	gcc

#FLAGS	=	-Wno-stack-protector -g -Wall -Wextra -Werror
FLAGS	=	-Wall -Wextra -Werror

INCSDIR	=	-I includes/ -I libft/

DIR		=	srcs/

SRCS	=	$(DIR)ft_print.c $(DIR)ft_utilities.c $(DIR)ft_env.c $(DIR)ft_archive.c \
				$(DIR)ft_architecture.c
SRCS1	=	$(DIR)ft_nm.c $(DIR)ft_nm_routine32.c $(DIR)ft_nm_routine64.c $(SRCS)
SRCS2	=	$(DIR)ft_otool.c $(DIR)ft_otool_routine32.c $(DIR)ft_otool_routine64.c $(SRCS)

OBJS1	=	$(SRCS1:.c=.o)
OBJS2	=	$(SRCS2:.c=.o)


all		:	$(NAME1) $(NAME2)

%.o		:	%.c
			@$(CC) $(FLAGS) -o $@ -c $< $(INCSDIR)

$(NAME1):	$(OBJS1)
			@make -C libft/
			@$(CC) -o $(NAME1) $(OBJS1) $(INCSDIR) -L libft/ -lft
			@echo "$(NAME1) created"

$(NAME2):	$(OBJS2)
			@make -C libft/
			@$(CC) -o $(NAME2) $(OBJS2) $(INCSDIR) -L libft/ -lft
			@echo "$(NAME2) created"

clean	:
			@make -C libft/ clean
			@rm -f $(OBJS1) $(OBJS2)

fclean	:	clean
			@make -C libft/ fclean
			@rm -f $(NAME1) $(NAME2)
			@echo "$(NAME1) and $(NAME2) deleted"

re		:	fclean all


.PHONY	:	all clean fclean re
