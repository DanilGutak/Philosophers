/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 15:00:38 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/08 21:29:53 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleep_improved(int time)
{
	size_t	finish;
	size_t	start;

	start = get_current_time();
	finish = start + time;
	while (start < finish)
	{
		usleep(100);
		start = get_current_time();
	}
}
void	take_right_fork(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->full != 1)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(&philos->r_fork);
		print_event(philos, "has taken a fork with right hand");
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}
void	take_left_fork(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->full != 1)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(philos->l_fork);
		print_event(philos, "has taken a fork with left hand");
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}
void	eat_procedure(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->full != 1)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		print_event(philos, "is eating");
		pthread_mutex_lock(philos->eaten_lock);
		philos->times_eaten++;
		philos->time_last_eat = get_current_time();
		pthread_mutex_unlock(philos->eaten_lock);
		sleep_improved(philos->time_eat);
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}
void	eat(t_philos *philos)
{
	if (philos->id % 2 == 0)
	{
		take_left_fork(philos);
		take_right_fork(philos);
		eat_procedure(philos);
		pthread_mutex_unlock(&philos->r_fork);
		pthread_mutex_unlock(philos->l_fork);
	}
	else
	{
		take_right_fork(philos);
		take_left_fork(philos);
		eat_procedure(philos);
		pthread_mutex_unlock(philos->l_fork);
		pthread_mutex_unlock(&philos->r_fork);
	}
}
void	sleeping(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->full != 1)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		print_event(philos, "is sleeping");
		sleep_improved(philos->time_sleep);
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}
void	*routine(void *s)
{
	t_philos	*philos;

	philos = (t_philos *)s;
	if (philos->num_phil == 1)
	{
		print_event(philos, "has taken a fork with right hand");
		return (sleep_improved(philos->time_die), NULL);
	}
	pthread_mutex_lock(philos->eaten_lock);
	while (*philos->dead != 1 && *philos->full != 1)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		eat(philos);
		sleeping(philos);
		pthread_mutex_lock(philos->eaten_lock);
		if (*philos->dead != 1 && *philos->full != 1)
		{
			pthread_mutex_unlock(philos->eaten_lock);
			print_event(philos, "is thinking");
		}
		else
			pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(philos->eaten_lock);
	}
	return (NULL);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
int	check_full(t_data *data)
{
	int	i;
	int	flag;

	flag = 0;
	i = -1;
	while (++i < data->squad[0].num_phil)
	{
		pthread_mutex_lock(&data->eaten_lock);
		if (data->squad[i].times_eaten == data->squad[i].times_to_eat)
			flag = 1;
		else
			flag = 0;
		pthread_mutex_unlock(&data->eaten_lock);
	}
	if (flag == 1)
	{
		pthread_mutex_lock(&data->eaten_lock);
		data->full = 1;
		pthread_mutex_unlock(&data->eaten_lock);
		pthread_mutex_lock(&data->print_lock);
		printf("All philosphers are full!\n");
		pthread_mutex_unlock(&data->print_lock);
		return (1);
	}
	return (0);
}
int	print_event(t_philos *data, char *str)
{
	pthread_mutex_lock(data->print_lock);
	printf("==%zu== Philosopher [%d] %s\n", get_current_time()
		- data->start_time, data->id, str);
	pthread_mutex_unlock(data->print_lock);
	return (1);
}
int	check_death(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->squad[0].num_phil)
	{
		pthread_mutex_lock(&data->eaten_lock);
		if (get_current_time()
			- data->squad[i].time_last_eat > data->squad[i].time_die)
			return (data->dead = 1, print_event(&data->squad[i], "died."));
		pthread_mutex_unlock(&data->eaten_lock);
	}
	return (0);
}

int	action(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->squad[0].num_phil)
	{
		data->squad[i].start_time = get_current_time();
		data->squad[i].time_last_eat = data->squad[i].start_time;
		if (pthread_create(&data->squad[i].thread, NULL, &routine,
				&data->squad[i]) != 0)
		{
			destroy_philosphers(data->squad, i);
			return (error("creating thread failed"));
		}
	}
	while (1)
	{
		if (check_death(data) == 1)
			return (0);
		if (check_full(data) == 1)
			return (0);
	}
	return (0);
}
