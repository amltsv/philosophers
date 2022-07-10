/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 03:31:12 by teevee            #+#    #+#             */
/*   Updated: 2021/03/31 04:25:50 by teevee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "one.h"
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

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

	pthread_mutex_lock(&ph->common->mutex[ph->left]);
	pthread_mutex_lock(&ph->common->mutex[ph->phn]);
	gettimeofday(&tv, 0);
	print_status(ph, "has taken a fork");
	print_status(ph, "is eating");
	ph->last_meal = tv.tv_sec;
	ph->ulast_meal = tv.tv_usec;
	usleep(ph->common->time_to_eat * 1000);
	pthread_mutex_unlock(&ph->common->mutex[ph->phn]);
	pthread_mutex_unlock(&ph->common->mutex[ph->left]);
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
	while (ph->common->n_eat == -1 ? 1 : (++ph->ate < ph->common->n_eat))
	{
		eating(ph);
		print_status(ph, "is sleeping");
		usleep(ph->common->time_to_sleep * 1000);
		print_status(ph, "is thinking");
	}
	ph->ready = 1;
	(ph->common->ready)++;
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
		ph[i].left = ((int)i - 1 < 0 ? n - 1 : i - 1);
		pthread_create(&(common.pthread[i]), 0, philosophers_life, &(ph[i]));
	}
	while (!usleep(5969 / n))
		if (common.ready == common.n)
			return ;
		else if (dead(&ph[i % n]) && !(ph[i % n].ready))
		{
			print_status(&ph[i++ % n], "is dead");
			return ;
		}
}

int		main(int ac, char **av)
{
	t_phil			*ph;
	t_common		common;
	size_t			i;
	size_t			n;

	if (ac < 5)
		return (write(1, "Not enough arguments!\n", 22));
	n = ft_atoll(av[1]);
	if (!(ph = (t_phil*)malloc(n * sizeof(t_phil))) ||
	!(common.mutex = (pthread_mutex_t*)malloc(n * sizeof(pthread_mutex_t))) ||
	!(common.pthread = (pthread_t*)malloc(n * sizeof(pthread_t))))
		return (write(1, "malloc error\n", 12));
	get_args(&common, ac, av);
	common.ready = 0;
	i = -1;
	while (++i < n)
		pthread_mutex_init(common.mutex + i, 0);
	pthread_mutex_init(&common.op_mutex, 0);
	control(n, ph, common);
	return (0);
}
