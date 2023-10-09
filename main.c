/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 12:03:27 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/09 17:04:39 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_argv(char **str)
{
	int	i;
	int	j;

	i = 1;
	while (str[i] != NULL)
	{
		j = 0;
		while (str[i][j] == ' ' || str[i][j] == '\v' || str[i][j] == '\t'
			|| str[i][j] == '\n' || str[i][j] == '\f' || str[i][j] == '\r')
			j++;
		if (!str[i][j])
			return (1);
		while (str[i][j])
		{
			if (!(str[i][j] >= '0' && str[i][j] <= '9'))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	parse_input(t_philos *philo, int argc, char **argv)
{
	philo->time_die = atoi_new(argv[2]);
	philo->time_eat = atoi_new(argv[3]);
	philo->time_sleep = atoi_new(argv[4]);
	philo->num_phil = atoi_new(argv[1]);
	if (argc == 6)
		philo->times_to_eat = atoi_new(argv[5]);
	else
		philo->times_to_eat = -1;
	if (philo->time_die == 9876543210 || philo->time_eat == 9876543210
		|| philo->time_sleep == 9876543210 || philo->times_to_eat == 9876543210
		|| philo->num_phil == 9876543210)
		return (error("args are overflow integer"));
	if (philo->num_phil > 200)
		return (error("too many philosophers!"));
	if (philo->num_phil < 1)
		return (error("zero philosophers!"));
	return (0);
}

int	init_phils(t_data *data, int argc, char **argv)
{
	int	i;

	i = -1;
	data->dead = 0;
	data->n_full = 0;
	if (parse_input(&(data->squad[0]), argc, argv) == 1)
		return (1);
	while (++i < data->squad[0].num_phil)
	{
		parse_input(&(data->squad[i]), argc, argv);
		data->squad[i].dead = &data->dead;
		data->squad[i].n_full = &data->n_full;
		data->squad[i].id = i;
		data->squad[i].times_eaten = 0;
		data->squad[i].print_lock = &data->print_lock;
		data->squad[i].eaten_lock = &data->eaten_lock;
		if (pthread_mutex_init(&data->squad[i].r_fork, NULL) != 0)
			return (destroy_forks(data->squad, i));
		if (i != 0)
			data->squad[i].l_fork = &data->squad[i - 1].r_fork;
	}
	data->squad[0].l_fork = &data->squad[data->squad[0].num_phil - 1].r_fork;
	return (0);
}

int	create_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (error("mutex print_lock failed"));
	if (pthread_mutex_init(&data->eaten_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&data->eaten_lock);
		return (error("mutex eaten_lock failed"));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philos	squad[200];

	data.squad = squad;
	if (argc > 6 || argc < 5)
		return (error("wrong num of args"));
	if (check_argv(argv) == 1)
		return (error("args are not numeric positive values without +"));
	if (create_mutexes(&data) == 1)
		return (1);
	if (init_phils(&data, argc, argv) == 1)
		return (1);
	if (action(&data) == 1)
		return (1);
	destroy_forks(data.squad, data.squad->num_phil);
	return (0);
}
