/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 13:35:16 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/11 16:23:43 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	time_to_var_ms(long *time_ms);
long	current_time(void);
void	set_status(int type, t_th_data *p_idata);
int		dead_check(t_th_data *p_idata);

int	sub_philo_cycle3(t_th_data *p_idata, t_misc *p_misc)
{
	printf("%ld	%d is sleeping\n", p_idata->current_time - \
			p_misc->start_time_ms, p_idata->id);
	sem_post(p_misc->print_sem);
	while (current_time() - p_idata->current_time < p_idata->p_misc->slp_tm \
			&& !dead_check(p_idata))
		usleep(499);
	time_to_var_ms(&(p_idata->current_time));
	if (dead_check(p_idata))
		return (0);
	sem_wait(p_misc->print_sem);
	if (dead_check(p_idata))
		return (sem_post(p_misc->print_sem), 0);
	printf("%ld	%d is thinking\n", p_idata->current_time - \
			p_misc->start_time_ms, p_idata->id);
	sem_post(p_misc->print_sem);
	return (0);
}

int	sub_philo_cycle2(t_th_data *p_idata, t_misc *p_misc)
{
	if (dead_check(p_idata))
		return (sem_post(p_misc->print_sem), sem_post(p_misc->fork_sem), \
				sem_post(p_misc->fork_sem), 0);
	printf("%ld	%d is eating\n", p_idata->current_time - \
			p_misc->start_time_ms, p_idata->id);
	sem_post(p_misc->print_sem);
	while (current_time() - p_idata->current_time < p_idata->p_misc->eat_tm && \
			!dead_check(p_idata))
		usleep(499);
	sem_wait(p_idata->eat_tm_sem);
	p_idata->eat_nbr++;
	sem_post(p_idata->eat_tm_sem);
	sem_post(p_misc->fork_sem);
	sem_post(p_misc->fork_sem);
	time_to_var_ms(&(p_idata->current_time));
	if (dead_check(p_idata))
		return (0);
	sem_wait(p_misc->print_sem);
	if (dead_check(p_idata))
		return (sem_post(p_misc->print_sem), 0);
	sub_philo_cycle3(p_idata, p_misc);
	return (0);
}

int	sub_philo_cycle(t_th_data *p_idata, t_misc *p_misc)
{
	if (p_idata->p_misc->philo_nbr == 1)
	{
		while (!dead_check(p_idata))
			usleep(499);
		return (sem_post(p_idata->p_misc->fork_sem), 0);
	}
	sem_wait(p_idata->p_misc->fork_sem);
	time_to_var_ms(&(p_idata->current_time));
	if (dead_check(p_idata))
		return (sem_post(p_misc->fork_sem), sem_post(p_misc->fork_sem), 0);
	sem_wait(p_idata->p_misc->print_sem);
	if (dead_check(p_idata))
		return (sem_post(p_misc->fork_sem), sem_post(p_misc->fork_sem), \
				sem_post(p_misc->print_sem), 0);
	printf("%ld	%d has taken a fork\n", p_idata->current_time - \
		p_idata->p_misc->start_time_ms, p_idata->id);
	sem_post(p_idata->p_misc->print_sem);
	sem_wait(p_idata->eat_tm_sem);
	p_idata->last_eat_time = p_idata->current_time;
	sem_post(p_idata->eat_tm_sem);
	if (dead_check(p_idata))
		return (sem_post(p_misc->fork_sem), sem_post(p_misc->fork_sem), 0);
	sem_wait(p_misc->print_sem);
	sub_philo_cycle2(p_idata, p_misc);
	return (0);
}

void	*philo_cycle(void *p_data)
{
	t_th_data	*p_idata;

	p_idata = p_data;
	if (p_idata->id % 2 == 0)
		usleep(5000);
	while (!dead_check(p_idata))
	{
		sem_wait(p_idata->p_misc->fork_sem);
		time_to_var_ms(&(p_idata->current_time));
		if (dead_check(p_idata))
			return (sem_post(p_idata->p_misc->fork_sem), (void *)0);
		sem_wait(p_idata->p_misc->print_sem);
		if (dead_check(p_idata))
			return (sem_post(p_idata->p_misc->print_sem), \
					sem_post(p_idata->p_misc->fork_sem), (void *)0);
		printf("%ld	%d has taken a fork\n", p_idata->current_time - \
				p_idata->p_misc->start_time_ms, p_idata->id);
		sem_post(p_idata->p_misc->print_sem);
		sub_philo_cycle(p_idata, p_idata->p_misc);
	}
	return (0);
}

int	philosopher(t_th_data *p_idata)
{
	time_to_var_ms(&(p_idata->last_eat_time));
	sem_wait(p_idata->p_misc->print_sem);
	printf("%ld	%d is thinking\n", current_time() - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	sem_post(p_idata->p_misc->print_sem);
	if (pthread_create(&(p_idata->thread), 0, philo_cycle, p_idata) != 0)
		return (printf("Error creating thread in philo %d\n", p_idata->id), 1);
	pthread_detach(p_idata->thread);
	while (p_idata->dead_time == -1)
	{
		usleep(4000);
		sem_wait(p_idata->eat_tm_sem);
		if (current_time() - p_idata->last_eat_time > p_idata->p_misc->die_tm)
			set_status(0, p_idata);
		if (p_idata->p_misc->eat_nbr > 0 && p_idata->eat_nbr >= \
				p_idata->p_misc->eat_nbr)
			set_status(-2, p_idata);
		sem_post(p_idata->eat_tm_sem);
	}
	if (p_idata->dead_time == -2)
		return (usleep(2000), 0);
	sem_wait(p_idata->p_misc->print_sem);
	printf("%ld	%d died\n", p_idata->dead_time - \
			p_idata->p_misc->start_time_ms, p_idata->id);
	return (1);
}
