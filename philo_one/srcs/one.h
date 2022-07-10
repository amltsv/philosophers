/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 03:17:54 by teevee            #+#    #+#             */
/*   Updated: 2021/03/31 04:24:46 by teevee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ONE_H
# define ONE_H

# include <pthread.h>

typedef struct		s_common
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n;
	int				n_eat;
	int				start;
	int				ms_start;
	int				simulation;
	int				ready;
	pthread_t		*pthread;
	pthread_mutex_t	op_mutex;
	pthread_mutex_t	*mutex;
}					t_common;

typedef struct		s_phil
{
	int				phn;
	long			last_meal;
	int				ulast_meal;
	int				left;
	int				ready;
	int				ate;
	t_common		*common;
	short			status;
}					t_phil;

long long			ft_atoll(const char *p_nbr);
int					get_args(t_common *philo, int ac, char **av);
int					print_status(t_phil *ph, char *s);
void				ft_putnbr_fd(int n, int fd);
#endif
