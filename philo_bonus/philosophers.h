/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:06:54 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/11 16:34:24 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_misc
{
	int		slp_tm;
	int		eat_tm;
	int		die_tm;
	int		philo_nbr;
	int		eat_nbr;
	long	start_time_ms;
	int		*pid_arr;
	sem_t	*print_sem;
	sem_t	*fork_sem;
}	t_misc;

typedef struct s_th_data
{
	int				id;
	long			last_eat_time;
	long			current_time;
	long			dead_time;
	int				eat_nbr;
	t_misc			*p_misc;
	pthread_t		thread;
	sem_t			*eat_tm_sem;
	sem_t			*dead_tm_sem;
	char			eat_sem_name[15];
	char			dead_sem_name[15];
}	t_th_data;

#endif
