/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 03:20:53 by teevee            #+#    #+#             */
/*   Updated: 2021/03/31 04:25:31 by teevee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "two.h"
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int		dead(t_phil *ph)
{
	struct timeval	tval;
	long			d;
	int				md;

	gettimeofday(&tval, 0);
	d = tval.tv_sec - ph->last_meal;
	md = (tval.tv_usec - ph->ulast_meal) / 1000;
	while (--d >= 0)
		md += 1000;
	if (md > ph->common->time_to_die)
		return (1);
	return (0);
}

int		eating(t_phil *ph)
{
	struct timeval	tv;

	sem_wait(ph->common->protect);
	sem_wait(ph->common->sem);
	sem_wait(ph->common->sem);
	sem_post(ph->common->protect);
	gettimeofday(&tv, 0);
	print_status(ph, "has taken a fork");
	print_status(ph, "is eating");
	ph->last_meal = tv.tv_sec;
	ph->ulast_meal = tv.tv_usec;
	usleep(ph->common->time_to_eat * 1000);
	sem_post(ph->common->sem);
	sem_post(ph->common->sem);
	return (0);
}

void	*philosophers_life(void *obj)
{
	t_phil			*ph;
	struct timeval	tval;

	ph = (t_phil*)obj;
	gettimeofday(&tval, 0);
	ph->last_meal = tval.tv_sec;
	ph->ulast_meal = tval.tv_usec;
	while (ph->common->n_eat == -1 ? 1 : (++(ph->ate) < ph->common->n_eat))
	{
		if (dead(ph))
			return (0);
		eating(ph);
		print_status(ph, "is sleeping");
		usleep(ph->common->time_to_sleep * 1000);
		print_status(ph, "is thinking");
	}
	ph->ready = 1;
	ph->common->ready++;
	return (0);
}

void	control(size_t n, t_phil *ph, t_common common)
{
	size_t	i;

	i = -1;
	while (++i < n)
	{
		ph[i].common = &common;
		ph[i].phn = i;
		ph[i].status = 1;
		ph[i].ate = -1;
		ph[i].ready = 0;
		pthread_create(&(common.pthread[i]), 0, philosophers_life, &(ph[i]));
	}
	i = -1;
	while (!usleep(6969 / n))
		if (common.ready == common.n)
			return ;
		else if (dead(&ph[i % n]) && !ph[i % n].ready)
		{
			print_status(&ph[i % n], "is dead");
			return ;
		}
}

int		main(int ac, char **av)
{
	t_phil			*ph;
	t_common		common;
	size_t			n;

	sem_unlink("/sem");
	sem_unlink("/op_sem");
	sem_unlink("/protect");
	if (ac < 5)
		return (write(1, "Not enough arguments!\n", 22));
	n = ft_atoll(av[1]);
	if (!(ph = (t_phil*)malloc(n * sizeof(t_phil))) ||
	!(common.pthread = (pthread_t*)malloc(n * sizeof(pthread_t))))
		return (write(1, "malloc error\n", 12));
	get_args(&common, ac, av);
	common.ready = 0;
	if ((common.sem = sem_open("/sem", O_CREAT, 0660, n)) == SEM_FAILED
	|| (common.op_sem = sem_open("/op_sem", O_CREAT, 0660, 1)) == SEM_FAILED ||
	(common.protect = sem_open("/protect", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (write(1, "malloc error\n", 12));
	control(n, ph, common);
	sem_unlink("/sem");
	sem_unlink("/op_sem");
	sem_unlink("/protect");
	return (0);
}
