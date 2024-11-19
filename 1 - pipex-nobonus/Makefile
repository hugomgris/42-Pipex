# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/29 11:12:41 by hmunoz-g          #+#    #+#              #
#    Updated: 2024/11/15 16:41:50 by hmunoz-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
DEF_COLOR = \033[0;39m
YELLOW = \033[0;93m
CYAN = \033[0;96m
GREEN = \033[0;92m
BLUE = \033[0;94m
RED = \033[0;91m

NAME = pipex
CC = cc
FLAGS = -Werror -Wall -Wextra -g
LIBFTDIR = libft/
PRINTFDIR = libft/ft_printf/
RM = rm -f

SRCS = pipex.c pipex_utils.c

OBJS = $(SRCS:.c=.o)

all: $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a $(NAME)

%.o: %.c Makefile pipex.h #libft/libft.h libft/ft_printf/ft_printf.h
	$(CC) $(FLAGS) -Ilibft -Ilibft/ft_printf -c $< -o $@
	@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"

$(NAME): $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a $(OBJS)
	@echo "$(GREEN)Compiling pipex!$(DEF_COLOR)"
	$(CC) $(FLAGS) pipex.c pipex_utils.c $(LIBFTDIR)libft.a $(PRINTFDIR)libftprintf.a -o pipex
	@echo "$(GREEN)Pipex compiled!$(DEF_COLOR)"

$(LIBFTDIR)libft.a:
	@echo "$(CYAN)Building libft.a!$(DEF_COLOR)"
	@$(MAKE) -C $(LIBFTDIR)

$(PRINTFDIR)libftprintf.a:
	@echo "$(CYAN)Building libftprintf.a!$(DEF_COLOR)"
	@$(MAKE) -C $(PRINTFDIR)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@$(MAKE) clean -C $(PRINTFDIR)
	@$(RM) $(OBJS)
	@echo "$(RED)Cleaned object files$(DEF_COLOR)"

fclean: clean
	@$(MAKE) fclean -C $(LIBFTDIR)
	@$(MAKE) fclean -C $(PRINTFDIR)
	@$(RM) pipex
	@echo "$(RED)Cleaned all binaries$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re libft ft_printf
