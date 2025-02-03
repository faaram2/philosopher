/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 19:06:54 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/10 13:07:02 by ayeganya         ###   ########.fr       */
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

typedef struct s_misc
{
	int				slp_tm;
	int				eat_tm;
	int				die_tm;
	int				status;
	int				philo_nbr;
	int				eat_nbr;
	long			start_time_ms;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	status_mtx;
}	t_misc;

typedef struct s_th_data
{
	int					id;
	pthread_t			thread;
	long				last_eat_time;
	long				current_time;
	int					eat_nbr;
	pthread_mutex_t		*fork1;
	pthread_mutex_t		*fork2;
	pthread_mutex_t		eat_tm_mtx;
	t_misc				*p_misc;
}	t_th_data;
/*
int		my_atoi(char *str, int *nbr);
void	time_to_var_ms(long *time_ms);
long	current_time(void);
void	mem_clean(t_misc *p_misc, t_th_data *p_data);
void	*philosopher(void *p_data);
int	init_init(int argc, char *argv[], t_misc **pp_misc);
int philo_data_init(t_th_data **pp_data, t_misc *p_misc);
void    sub_main(t_th_data *p_data, t_misc *p_misc);
int sub_philo_cycle2(t_th_data *p_idata);
int sub_philo_cycle(t_th_data *p_idata);
int philo_cycle(t_th_data *p_idata);*/

#endif
