/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 03:17:38 by teevee            #+#    #+#             */
/*   Updated: 2021/03/31 04:24:57 by teevee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREE_H
# define THREE_H

# include <pthread.h>
# include <semaphore.h>

typedef struct		s_common
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				start;
	int				ms_start;
	int				simulation;
	int				n;
	int				n_eat;
	int				ready;
	pid_t			*pid;
	sem_t			*sem;
	sem_t			*op_sem;
	sem_t			*protect;
}					t_common;

typedef struct		s_phil
{
	int				phn;
	long			last_meal;
	int				ulast_meal;
	int				ate;
	t_common		*common;
	short			status;
}					t_phil;

long long			ft_atoll(const char *p_nbr);
int					get_args(t_common *philo, int ac, char **av);
void				print_status(t_phil *ph, char *s);
void				ft_putnbr_fd(int n, int fd);
void				*observer(void *obj);
int					dead(t_phil *ph);
#endif
