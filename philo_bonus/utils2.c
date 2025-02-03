/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:28:42 by ayeganya          #+#    #+#             */
/*   Updated: 2024/08/11 16:30:51 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	current_time(void);

void	my_itoa(int nbr, char **p_str)
{
	if (nbr >= 10)
		my_itoa(nbr / 10, p_str);
	**p_str = nbr % 10 + '0';
	*p_str += 1;
}

void	sem_name_gen(char *result, char *str1, int nbr)
{
	char	str_nbr[5];
	char	*p_str_nbr[1];
	int		i;
	int		j;

	i = 0;
	*p_str_nbr = str_nbr;
	while (i < 5)
		str_nbr[i++] = 0;
	my_itoa(nbr, p_str_nbr);
	i = 0;
	while (str1[i])
	{
		result[i] = str1[i];
		i++;
	}
	j = 0;
	while (str_nbr[j])
		result[i++] = str_nbr[j++];
}

void	my_sem_unlink(t_th_data *p_data, int nbr)
{
	int	i;

	i = 0;
	while (i < nbr)
	{
		sem_unlink(p_data[i].eat_sem_name);
		sem_unlink(p_data[i].dead_sem_name);
		i++;
	}
}

void	set_status(int type, t_th_data *p_idata)
{
	if (type == -2)
	{
		sem_wait(p_idata->dead_tm_sem);
		p_idata->dead_time = -2;
		sem_post(p_idata->dead_tm_sem);
		return ;
	}
	sem_wait(p_idata->dead_tm_sem);
	p_idata->dead_time = current_time();
	sem_post(p_idata->dead_tm_sem);
}

int	dead_check(t_th_data *p_idata)
{
	sem_wait(p_idata->dead_tm_sem);
	if (p_idata->dead_time == -1)
	{
		sem_post(p_idata->dead_tm_sem);
		return (0);
	}
	else
	{
		sem_post(p_idata->dead_tm_sem);
		return (1);
	}
}
