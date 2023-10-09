/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 15:00:38 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/09 17:22:39 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_event(t_philos *data, char *str)
{
	pthread_mutex_lock(data->print_lock);
	printf("%zu %d %s\n", get_current_time()
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
		{
			data->dead = 1;
			if (data->n_full < data->squad[i].num_phil)
				print_event(&data->squad[i], "died");
			else
				print_event(&data->squad[i], "ALL FULL");
			pthread_mutex_unlock(&data->eaten_lock);
			return (1);
		}
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
		if (check_death(data) == 1)
			return (0);
	i = -1;
	while (++i < data->squad[0].num_phil)
	{
		pthread_join(data->squad[i].thread, NULL);
	}
	return (0);
}
