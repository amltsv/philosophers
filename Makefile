# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/31 03:11:32 by teevee            #+#    #+#              #
#    Updated: 2021/03/31 03:40:00 by teevee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

P1 = philo_one
P2 = philo_two
P3 = philo_three

FLAGS = -Wall -Wextra -Werror -pthread
DIR_1 = ./philo_one/srcs/
FILE_1 = main.c help.c

DIR_2 = ./philo_two/srcs/
FILE_2 = main.c help.c

DIR_3 = ./philo_three/srcs/
FILE_3 = main.c help.c

F1 = $(addprefix $(DIR_1), $(FILE_1))
F2 = $(addprefix $(DIR_2), $(FILE_2))
F3 = $(addprefix $(DIR_3), $(FILE_3))

all:
	gcc  $(FLAGS) $(F1) -o $(P1)/$(P1)
	gcc  $(FLAGS) $(F2) -o $(P2)/$(P2)
	gcc  $(FLAGS) $(F3) -o $(P3)/$(P3)

fclean:
	rm -f $(P1)/$(P1) $(P2)/$(P2) $(P3)/$(P3)

re: fclean all

.PHONY: all clean fclean re
