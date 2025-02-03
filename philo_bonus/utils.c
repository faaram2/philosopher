/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:13:52 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/11 16:31:48 by ayeganya         ###   ########.fr       */
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
	if (p_misc)
	{
		free(p_misc->pid_arr);
		sem_close(p_misc->print_sem);
		sem_close(p_misc->fork_sem);
		free(p_misc);
	}
	if (p_data == 0)
		return ;
	free(p_data);
}
