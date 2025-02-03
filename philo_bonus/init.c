/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:21:36 by ayeganya          #+#    #+#             */
/*   Updated: 2024/10/02 15:19:25 by ayeganya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		my_atoi(char *str, int *nbr);
void	time_to_var_ms(long *time_ms);
long	current_time(void);
void	mem_clean(t_misc *p_misc, t_th_data *p_data);
int		philosopher(t_th_data *p_data);
void	sem_name_gen(char *result, char *str1, int nbr);
void	my_sem_unlink(t_th_data *p_data, int nbr);

int	in_in_2(t_misc **pp_misc)
{
	sem_unlink("print");
	sem_unlink("fork");
	(*pp_misc)->print_sem = sem_open("print", O_CREAT, 0600, 1);
	(*pp_misc)->fork_sem = sem_open("fork", O_CREAT, 0600, \
			(*pp_misc)->philo_nbr);
	if ((*pp_misc)->print_sem == SEM_FAILED || \
			(*pp_misc)->fork_sem == SEM_FAILED)
		return (free((*pp_misc)->pid_arr), free(*pp_misc), \
				printf("Semaphore init error\n"), -1);
	return (0);
}

int	in_in(int argc, char *argv[], t_misc **pp_misc)
{
	*pp_misc = (t_misc *)malloc(sizeof(t_misc));
	if (!*pp_misc)
		return (printf("malloc error\n"), -1);
	(*pp_misc)->eat_nbr = -1;
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
	(*pp_misc)->pid_arr = (int *)malloc(sizeof(int) * (*pp_misc)->philo_nbr);
	if ((*pp_misc)->pid_arr == 0)
		return (free(*pp_misc), printf("malloc error\n"), -1);
	return (in_in_2(pp_misc));
}

int	ph_dt_in(t_th_data **pp_data, t_misc *p_misc, int i)
{
	*pp_data = (t_th_data *)malloc(sizeof(t_th_data) * p_misc->philo_nbr);
	if (!*pp_data)
		return (mem_clean(p_misc, 0), sem_unlink("print"), sem_unlink("fork"), \
				printf("malloc error\n"), -1);
	while (i < p_misc->philo_nbr)
	{
		(*pp_data)[i].p_misc = p_misc;
		(*pp_data)[i].id = i + 1;
		(*pp_data)[i].eat_nbr = 0;
		(*pp_data)[i].dead_time = -1;
		sem_name_gen((*pp_data)[i].eat_sem_name, "eat_sem_", i);
		sem_name_gen((*pp_data)[i].dead_sem_name, "dead_sem_", i);
		sem_unlink((*pp_data)[i].eat_sem_name);
		sem_unlink((*pp_data)[i].dead_sem_name);
		(*pp_data)[i].eat_tm_sem = sem_open((*pp_data)[i].eat_sem_name, \
				O_CREAT, 0600, 1);
		(*pp_data)[i].dead_tm_sem = sem_open((*pp_data)[i].dead_sem_name, \
				O_CREAT, 0600, 1);
		if ((*pp_data)[i].eat_tm_sem == SEM_FAILED || \
				(*pp_data)[i].dead_tm_sem == SEM_FAILED)
			return (my_sem_unlink(*pp_data, i), mem_clean(p_misc, *pp_data), \
					printf("semaphore creation error\n"), 0);
		i++;
	}
	return (0);
}

void	philo_wait(t_misc *p_misc)
{
	int	i;
	int	pid;

	while (1)
	{
		pid = waitpid(-1, &i, 0);
		if (pid != -1 && WIFEXITED(i) && WEXITSTATUS(i) == 1)
		{
			i = 0;
			while (i < p_misc->philo_nbr)
				if (p_misc->pid_arr[i++] != pid)
					kill(p_misc->pid_arr[i - 1], SIGKILL);
			break ;
		}
		if (pid == -1)
			break ;
	}
}

int	main(int argc, char *argv[])
{
	t_th_data	*p_data;
	t_misc		*p_misc;
	int			i;
	int			pid;

	if (argc < 5)
		return (printf("Please specify all necessary arguments\n"), 1);
	if (in_in(argc, argv, &p_misc) == -1 || ph_dt_in(&p_data, p_misc, 0) == -1)
		return (1);
	i = 0;
	time_to_var_ms(&(p_misc->start_time_ms));
	while (i < p_misc->philo_nbr)
	{
		pid = fork();
		if (pid == 0)
		{
			i = philosopher(&p_data[i]);
			return (usleep(200), mem_clean(p_misc, p_data), i);
		}
		(p_misc->pid_arr)[i++] = pid;
	}
	philo_wait(p_misc);
	return (my_sem_unlink(p_data, p_misc->philo_nbr), \
		mem_clean(p_misc, p_data), sem_unlink("print"), sem_unlink("fork"));
}
