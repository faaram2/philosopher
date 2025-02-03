/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:13:52 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/01 15:17:20 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	my_atoi(char *str, int *nbr)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	if (str == 0 || *str == 0)
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + str[i] - '0';
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	*nbr = (int)result;
	return (0);
}

void	time_to_var_ms(long *time_ms)
{
	struct timeval	start;

	gettimeofday (&start, 0);
	*time_ms = start.tv_sec * 1000 + start.tv_usec / 1000;
}

void	time_to_var_ms1(long *time_ms, t_th_data *p_idata)
{
	struct timeval	start;

	gettimeofday (&start, 0);
	pthread_mutex_lock(&p_idata->eat_tm_mtx);
	*time_ms = start.tv_sec * 1000 + start.tv_usec / 1000;
	pthread_mutex_unlock(&p_idata->eat_tm_mtx);
}

long	current_time(void)
{
	struct timeval	c_time;
	long			time_ms;

	gettimeofday (&c_time, 0);
	time_ms = c_time.tv_sec * 1000 + c_time.tv_usec / 1000;
	return (time_ms);
}

void	mem_clean(t_misc *p_misc, t_th_data *p_data)
{
	int	i;

	i = 0;
	while (i < p_misc->philo_nbr)
	{
		if (p_data[i].fork1)
		{
			pthread_mutex_destroy(p_data[i].fork1);
			free(p_data[i].fork1);
		}
		i++;
	}
	pthread_mutex_destroy(&(p_misc->print_mtx));
	pthread_mutex_destroy(&(p_misc->status_mtx));
	free(p_data);
	free(p_misc);
}
