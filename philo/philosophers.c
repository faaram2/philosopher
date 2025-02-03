/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 20:04:56 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/11 17:08:29 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		my_atoi(char *str, int *nbr);
void	time_to_var_ms(long *time_ms);
void	time_to_var_ms1(long *time_ms, t_th_data *p_idata);
long	current_time(void);
void	mem_clean(t_misc *p_misc, t_th_data *p_data);

int	check_status(t_th_data *p_idata)
{
	int	r_val;

	r_val = 0;
	pthread_mutex_lock(&(p_idata->p_misc->status_mtx));
	if (p_idata->p_misc->status == -1)
		r_val = 1;
	pthread_mutex_unlock(&(p_idata->p_misc->status_mtx));
	return (r_val);
}

int	sub_philo_cycle2(t_th_data *p_idata)
{
	while (current_time() - p_idata->current_time < p_idata->p_misc->slp_tm)
		usleep(499);
	time_to_var_ms(&(p_idata->current_time));
	pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
	if (!check_status(p_idata))
		return (pthread_mutex_unlock(&(p_idata->p_misc->print_mtx)), 0);
	printf("%ld	%d is thinking\n", p_idata->current_time - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
	return (0);
}

int	sub_philo_cycle(t_th_data *p_idata)
{
	pthread_mutex_lock(&p_idata->eat_tm_mtx);
	p_idata->last_eat_time = p_idata->current_time;
	pthread_mutex_unlock(&p_idata->eat_tm_mtx);
	pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
	if (!check_status(p_idata))
		return (pthread_mutex_unlock(&(p_idata->p_misc->print_mtx)), \
pthread_mutex_unlock(p_idata->fork1), pthread_mutex_unlock(p_idata->fork2), 0);
	printf("%ld	%d is eating\n", p_idata->current_time - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
	pthread_mutex_lock(&p_idata->eat_tm_mtx);
	p_idata->eat_nbr++;
	pthread_mutex_unlock(&p_idata->eat_tm_mtx);
	while (current_time() - p_idata->current_time < p_idata->p_misc->eat_tm)
		usleep(499);
	pthread_mutex_unlock(p_idata->fork1);
	pthread_mutex_unlock(p_idata->fork2);
	time_to_var_ms(&(p_idata->current_time));
	pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
	if (!check_status(p_idata))
		return (pthread_mutex_unlock(&(p_idata->p_misc->print_mtx)), 0);
	printf("%ld	%d is sleeping\n", p_idata->current_time - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
	return (sub_philo_cycle2(p_idata), 0);
}

int	philo_cycle(t_th_data *p_idata)
{
	while (check_status(p_idata))
	{
		pthread_mutex_lock(p_idata->fork1);
		time_to_var_ms(&(p_idata->current_time));
		pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
		if (!check_status(p_idata))
			return (pthread_mutex_unlock(&(p_idata->p_misc->print_mtx)), \
					pthread_mutex_unlock(p_idata->fork1), 0);
		printf("%ld	%d has taken a fork\n", p_idata->current_time - \
				p_idata->p_misc->start_time_ms, p_idata->id);
		pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
		if (p_idata->p_misc->philo_nbr == 1)
			return (pthread_mutex_unlock(p_idata->fork1), 0);
		pthread_mutex_lock(p_idata->fork2);
		time_to_var_ms(&(p_idata->current_time));
		pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
		if (!check_status(p_idata))
			return (pthread_mutex_unlock(&(p_idata->p_misc->print_mtx)), \
pthread_mutex_unlock(p_idata->fork1), pthread_mutex_unlock(p_idata->fork2), 0);
		printf("%ld	%d has taken a fork\n", p_idata->current_time - \
				p_idata->p_misc->start_time_ms, p_idata->id);
		pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
		sub_philo_cycle(p_idata);
	}
	return (0);
}

void	*philosopher(void *p_data)
{
	t_th_data	*p_idata;

	p_idata = p_data;
	pthread_mutex_lock(&p_idata->eat_tm_mtx);
	time_to_var_ms(&(p_idata->last_eat_time));
	pthread_mutex_unlock(&p_idata->eat_tm_mtx);
	pthread_mutex_lock(&(p_idata->p_misc->print_mtx));
	printf("%ld	%d is thinking\n", current_time() - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	pthread_mutex_unlock(&(p_idata->p_misc->print_mtx));
	if (p_idata->id % 2 == 0)
		usleep(3000);
	philo_cycle(p_idata);
	return (0);
}
