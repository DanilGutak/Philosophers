/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 17:58:53 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/06 17:04:07 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philos
{
	int				id;
	long int		time_die;
	long int		time_eat;
	long int		time_sleep;
	long int		times_to_eat;
	long int		time_since_eat;
	long int		times_eaten;
	long int		num_phil;
	int				*dead;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*eaten_lock;
}					t_philos;

typedef struct s_data
{
	t_philos		*squad;
	int				dead;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	eaten_lock;
}					t_data;

long int			atoi_new(char *str);
int					error(char *str);

#endif