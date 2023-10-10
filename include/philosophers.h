/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgutak <dgutak@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 17:58:53 by dgutak            #+#    #+#             */
/*   Updated: 2023/10/10 13:11:03 by dgutak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philos
{
	int				id;
	pthread_t		thread;
	size_t			time_die;
	size_t			time_eat;
	size_t			time_sleep;
	size_t			times_to_eat;
	size_t			time_last_eat;
	size_t			start_time;
	size_t			times_eaten;
	long int		num_phil;
	int				*dead;
	int				*n_full;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*eaten_lock;
}					t_philos;

typedef struct s_data
{
	t_philos		*squad;
	int				dead;
	int				n_full;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	eaten_lock;
}					t_data;

size_t				atoi_new(char *str);
int					error(char *str);
void				*routine(void *s);
int					action(t_data *data);
int					ft_putstr_fd(char *s, int fd);
void				destroy_forks(t_philos *philos, int i);
void				destroy_philosphers(t_philos *philos, int i);
int					print_event(t_philos *data, char *str);
size_t				get_current_time(void);
void				sleeping(t_philos *philos);
void				sleep_improved(int time);

#endif