/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:59:42 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/10 13:11:47 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s)
		return (write(1, "(null)", 6));
	while (*s)
	{
		write(fd, s, 1);
		s++;
		i++;
	}
	return (i);
}

size_t	atoi_new(char *str)
{
	size_t	x;
	int		sign;

	x = 0;
	sign = 1;
	while (*str == ' ' || *str == '\v' || *str == '\t' || *str == '\n'
		|| *str == '\f' || *str == '\r')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			x = x * 10 + *str - '0';
		else
			break ;
		if ((x > 2147483647 && sign == 1) || (sign == -1))
			return (9876543210);
		str++;
	}
	return (sign * x);
}

void	sleep_improved(int time)
{
	size_t	finish;
	size_t	start;

	start = get_current_time();
	finish = start + time;
	while (start < finish)
	{
		usleep(500);
		start = get_current_time();
	}
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	sleeping(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->n_full < philos->num_phil)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		print_event(philos, "is sleeping");
		sleep_improved(philos->time_sleep);
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}
