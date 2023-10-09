/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:06:05 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/09 18:12:24 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_right_fork(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->n_full < philos->num_phil)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(&philos->r_fork);
		print_event(philos, "has taken a fork");
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}

void	take_left_fork(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->n_full < philos->num_phil)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(philos->l_fork);
		print_event(philos, "has taken a fork");
	}
	else
		pthread_mutex_unlock(philos->eaten_lock);
}

void	eat_procedure(t_philos *philos)
{
	pthread_mutex_lock(philos->eaten_lock);
	if (*philos->dead != 1 && *philos->n_full < philos->num_phil)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		print_event(philos, "is eating");
		pthread_mutex_lock(philos->eaten_lock);
		philos->times_eaten++;
		if (philos->times_eaten >= philos->times_to_eat)
		{
			*philos->n_full += 1;
		}
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

void	*routine(void *s)
{
	t_philos	*philos;

	philos = (t_philos *)s;
	if (philos->num_phil == 1)
	{
		print_event(philos, "has taken a fork");
		return (sleep_improved(philos->time_die), NULL);
	}
	pthread_mutex_lock(philos->eaten_lock);
	while (*philos->dead == 0 && *philos->n_full < philos->num_phil)
	{
		pthread_mutex_unlock(philos->eaten_lock);
		eat(philos);
		sleeping(philos);
		pthread_mutex_lock(philos->eaten_lock);
		if (*philos->dead != 1 && *philos->n_full < philos->num_phil)
		{
			pthread_mutex_unlock(philos->eaten_lock);
			print_event(philos, "is thinking");
			usleep(1000);
		}
		else
			pthread_mutex_unlock(philos->eaten_lock);
		pthread_mutex_lock(philos->eaten_lock);
	}
	pthread_mutex_unlock(philos->eaten_lock);
	return (NULL);
}
