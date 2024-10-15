/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:01:15 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/14 18:48:35 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	arguments_are_not_valid(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) == 0)
		return (true);
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (true);
			j++;
		}
		i++;
	}
	return (false);
}

bool	cant_init_semaphore(t_philo **philo)
{
	(*philo)->shared->pid_tab = malloc(sizeof(pid_t) * ((*philo)->total_philos));
	if (!(*philo)->shared->pid_tab)
		return (true);
	(*philo)->shared->pid_tab_has_been_created = 1;
	(*philo)->shared->forks = sem_open(FSEM, O_CREAT, 0777,
			(*philo)->total_philos);
	if ((*philo)->shared->forks == SEM_FAILED)
		return (true);
	(*philo)->shared->forks_have_been_created = 1;
	(*philo)->shared->waiter = sem_open(FSEM, O_CREAT, 0777,
			(*philo)->total_philos / 2);
	if ((*philo)->shared->waiter == SEM_FAILED)
		return (true);
	(*philo)->shared->waiter_sem_have_been_created = 1;
	(*philo)->shared->print_sem = sem_open(PSEM, O_CREAT, 0777, 1);
	if ((*philo)->shared->print_sem == SEM_FAILED)
		return (true);
	(*philo)->shared->print_sem_have_been_created = 1;
	(*philo)->shared->death_sem = sem_open(DSEM, O_CREAT, 0777, 1);
	if ((*philo)->shared->death_sem == SEM_FAILED)
		return (true);
	(*philo)->shared->death_sem_have_been_created = 1;
	return (false);
}