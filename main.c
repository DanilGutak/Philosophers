/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 12:03:27 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/04 17:47:53 by dgutak           ###   ########.fr       */
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

int	parse_input(t_data *data, int argc, char **argv, int i)
{
	data->squad[i].time_die = atoi_new(argv[2]);
	data->squad[i].time_eat = atoi_new(argv[3]);
	data->squad[i].time_sleep = atoi_new(argv[4]);
	data->squad[i].num_phil = atoi_new(argv[1]);
	if (argc == 6)
		data->squad[i].times_to_eat = atoi_new(argv[5]);
	else
		data->squad[i].times_to_eat = -1;
	if (data->squad[i].time_die == 9876543210
		|| data->squad[i].time_eat == 9876543210
		|| data->squad[i].time_sleep == 9876543210
		|| data->squad[i].times_to_eat == 9876543210
		|| data->squad[i].num_phil == 9876543210)
		return (error("args are overflow integer"));
	if (data->squad[i].num_phil > 200)
		return (error("too many philosophers!"));
	if (data->squad[i].num_phil < 1)
		return (error("zero philosophers!"));
	return (0);
}

int	init_phils(t_data *data, int argc, char **argv)
{
	int	i;

	i = -1;
	if (parse_input(data, argc, argv, 0) == 1)
		return (1);
	while (++i < data->squad[0].num_phil)
	{
		parse_input(data, argc, argv, i);
		data->squad[i].id = i;
		pthread_mutex_init(&data->squad[i].r_fork, NULL);
		if (i != 0)
			data->squad[i].l_fork = &data->squad[i - 1].r_fork;
		printf("{%ld} ", data->squad[i].time_die);
	}
	data->squad[0].l_fork = &data->squad[data->squad[0].num_phil - 1].r_fork;
	printf("\n");
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philos	squad[200];
	int			i;

	data.squad = squad;
	if (argc > 6 || argc < 5)
		return (error("wrong num of args"));
	if (check_argv(argv) == 1)
		return (error("args are not numeric positive values without +"));
	if (init_phils(&data, argc, argv) == 1)
		return (1);
	i = -1;
	while (++i < data.squad[0].num_phil)
	{
		printf("{%ld} ", data.squad[i].time_die);



		
	}
	return (0);
}
