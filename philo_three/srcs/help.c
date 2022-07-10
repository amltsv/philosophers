/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teevee <teevee@students.21-school.ru>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 03:20:44 by teevee            #+#    #+#             */
/*   Updated: 2021/03/31 04:25:45 by teevee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "three.h"
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

long long	ft_atoll(const char *p_nbr)
{
	int			flag;
	long long	out;

	flag = 1;
	while (*p_nbr == ' ' || *p_nbr == '\t' || *p_nbr == '\r' ||
		*p_nbr == '\v' || *p_nbr == '\f' || *p_nbr == '\n')
		p_nbr++;
	if (*p_nbr == '+' || *p_nbr == '-')
	{
		if (*p_nbr == '-')
			flag = -1;
		p_nbr++;
	}
	out = 0;
	while (*p_nbr <= '9' && *p_nbr >= '0')
	{
		if (out * flag * 10 < out * flag)
			return (flag < 0 ? 0 : -1);
		out = out * 10;
		out = out + flag * (*p_nbr++ - '0');
	}
	return (out);
}

int			get_args(t_common *php, int ac, char **av)
{
	struct timeval	tv;

	php->n = ft_atoll(av[1]);
	php->time_to_die = ft_atoll(av[2]);
	php->time_to_eat = ft_atoll(av[3]);
	php->time_to_sleep = ft_atoll(av[4]);
	if (ac > 5)
		php->n_eat = ft_atoll(av[5]);
	else
		php->n_eat = -1;
	gettimeofday(&tv, 0);
	php->start = tv.tv_sec;
	php->ms_start = tv.tv_usec;
	return (0);
}

void		print_status(t_phil *ph, char *s)
{
	int				mtime;
	int				time;
	struct timeval	tv;

	gettimeofday(&tv, 0);
	mtime = (tv.tv_usec - ph->common->ms_start) / 1000;
	time = tv.tv_sec - ph->common->start;
	while (--time >= 0)
		mtime += 1000;
	sem_wait(ph->common->op_sem);
	ft_putnbr_fd(mtime, 1);
	write(1, " ", 1);
	ft_putnbr_fd(ph->phn + 1, 1);
	write(1, " ", 1);
	while (*s)
		write(1, s++, 1);
	write(1, "\n", 1);
	sem_post(ph->common->op_sem);
}

void		ft_putnbr_fd(int n, int fd)
{
	char	str[10];
	int		i;

	i = 10;
	if (n < 0)
	{
		write(fd, "-", 1);
		if (n == ((-1) * 2147483648))
		{
			str[--i] = '8';
			n = n / 10;
		}
		n = -n;
	}
	if (n == 0)
		write(fd, "0", 1);
	while (n != 0)
	{
		str[--i] = '0' + (n % 10);
		n = n / 10;
	}
	while (i < 10)
		write(fd, &str[i++], 1);
}

void		*observer(void *obj)
{
	t_phil	*ph;
	int		i;

	ph = (t_phil*)obj;
	while (!usleep(6969))
	{
		if (dead(ph))
		{
			i = -1;
			while (++i < ph->common->n)
				if (i != ph->phn)
					kill(ph->common->pid[i], SIGINT);
			exit(6969);
		}
	}
	return ((void*)0);
}
