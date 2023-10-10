/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:12:47 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/10 12:41:20 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	error(char *str)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

void	destroy_philosphers(t_philos *philos, int i)
{
	destroy_forks(philos, philos->num_phil);
	while (i >= 0)
	{
		pthread_join(philos[i].thread, NULL);
		i--;
	}
}

void	destroy_forks(t_philos *philos, int i)
{
	while (--i >= 0)
	{
		pthread_mutex_destroy(&philos[i].r_fork);
	}
	pthread_mutex_destroy(philos->eaten_lock);
	pthread_mutex_destroy(philos->print_lock);
}
