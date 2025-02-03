/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:21:36 by ayeganya          #+#    #+#             */
/*   Updated: 2024/10/02 15:19:09 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		my_atoi(char *str, int *nbr);
void	time_to_var_ms(long *time_ms);
long	current_time(void);
void	mem_clean(t_misc *p_misc, t_th_data *p_data);
void	*philosopher(void *p_data);

int	init_init(int argc, char *argv[], t_misc **pp_misc)
{
	*pp_misc = (t_misc *)malloc(sizeof(t_misc));
	if (!*pp_misc)
		return (printf("malloc error\r"), -1);
	(*pp_misc)->eat_nbr = -1;
	(*pp_misc)->status = -1;
	if (my_atoi(argv[1], &((*pp_misc)->philo_nbr)) == -1 || \
			(*pp_misc)->philo_nbr == 0)
		return (free(*pp_misc), \
				printf("Number of philosophers is incorrect\n"), -1);
	if (my_atoi(argv[2], &((*pp_misc)->die_tm)) == -1)
		return (free(*pp_misc), printf("Die time is incorrect\n"), -1);
	if (my_atoi(argv[3], &((*pp_misc)->eat_tm)) == -1)
		return (free(*pp_misc), printf("Eat time is incorrect\n"), -1);
	if (my_atoi(argv[4], &((*pp_misc)->slp_tm)) == -1)
		return (free(*pp_misc), printf("Sleep time is incorrect\n"), -1);
	if (argc == 6 && my_atoi(argv[5], &((*pp_misc)->eat_nbr)) == -1)
		return (free(*pp_misc), printf(\
			"Number of times each philosopher must eat is incorrect\n"), -1);
	if (argc != 6)
		(*pp_misc)->eat_nbr = -1;
	if (pthread_mutex_init(&((*pp_misc)->print_mtx), 0) || \
			pthread_mutex_init(&((*pp_misc)->status_mtx), 0))
		return (free(*pp_misc), printf("Mutex init error\n"), -1);
	return (0);
}

int	philo_data_init(t_th_data **pp_data, t_misc *p_misc, int i)
{
	*pp_data = (t_th_data *)malloc(sizeof(t_th_data) * p_misc->philo_nbr);
	if (!*pp_data)
		return (free(p_misc), printf("malloc error\n"), -1);
	(*pp_data)[0].fork1 = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!(*pp_data)[0].fork1 || pthread_mutex_init((*pp_data)[0].fork1, 0) || \
			pthread_mutex_init(&((*pp_data)[0].eat_tm_mtx), 0))
		return (free(p_misc), free(*pp_data), printf("mutex error\n"), -1);
	(*pp_data)[0].id = 1;
	(*pp_data)[0].p_misc = p_misc;
	(*pp_data)[0].eat_nbr = 0;
	while (i < p_misc->philo_nbr)
	{
		(*pp_data)[i].fork1 = \
					(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (!(*pp_data)[i].fork1 || pthread_mutex_init((*pp_data)[i].fork1, 0) \
				|| pthread_mutex_init(&((*pp_data)[i].eat_tm_mtx), 0))
			return (mem_clean(p_misc, *pp_data), printf("mutex error\n"), -1);
		(*pp_data)[i].p_misc = p_misc;
		(*pp_data)[i - 1].fork2 = (*pp_data)[i].fork1;
		(*pp_data)[i].id = i + 1;
		(*pp_data)[i++].eat_nbr = 0;
	}
	(*pp_data)[i - 1].fork2 = (*pp_data)[0].fork1;
	return (0);
}

void	set_status(int val, t_misc *p_misc)
{
	pthread_mutex_lock(&(p_misc->status_mtx));
	p_misc->status = val;
	pthread_mutex_unlock(&(p_misc->status_mtx));
}

void	sub_main(t_th_data *p_data, t_misc *p_misc)
{
	int	i;
	int	temp;

	while (p_misc->status == -1)
	{
		i = 0;
		temp = 1;
		usleep(1000);
		while (i < p_misc->philo_nbr)
		{
			pthread_mutex_lock(&((p_data[i]).eat_tm_mtx));
			if (current_time() - (p_data[i]).last_eat_time > p_misc->die_tm)
				set_status((p_data[i]).id, p_misc);
			if (p_misc->eat_nbr > 0 && p_data[i].eat_nbr <= p_misc->eat_nbr)
				temp = 0;
			pthread_mutex_unlock(&((p_data[i++]).eat_tm_mtx));
			if (p_misc->eat_nbr > 0 && temp == 1)
				set_status(-2, p_misc);
		}
	}
	pthread_mutex_lock(&(p_misc->print_mtx));
	if (p_misc->status > 0)
		printf("%ld	%d died\n", current_time() - p_misc->start_time_ms, \
				p_misc->status);
	pthread_mutex_unlock(&(p_misc->print_mtx));
}

int	main(int argc, char *argv[])
{
	t_th_data	*p_data;
	t_misc		*p_misc;
	int			i;

	if (argc < 5)
		return (printf("Please specify all necessary arguments\n"), 1);
	if (init_init(argc, argv, &p_misc) == -1)
		return (1);
	if (philo_data_init(&p_data, p_misc, 1) == -1)
		return (1);
	i = 0;
	time_to_var_ms(&(p_misc->start_time_ms));
	while (i++ < p_misc->philo_nbr)
		if (pthread_create(&(p_data[i - 1].thread), 0, philosopher, \
					&(p_data[i - 1])) != 0)
			return (mem_clean(p_misc, p_data), \
				printf("Error creating thread %d\n", i - 1), 1);
	sub_main(p_data, p_misc);
	i = 0;
	while (i < p_misc->philo_nbr)
		if (pthread_join(p_data[i++].thread, 0) != 0)
			return (mem_clean(p_misc, p_data), \
					printf("Error joining thread %d\n", i - 1), 1);
	return (mem_clean(p_misc, p_data), 0);
}
