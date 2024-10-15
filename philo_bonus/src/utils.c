/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 02:30:45 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/15 13:00:11 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	ft_atoi(char *str)
{
	int	num;
	int	i;

	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num);
}

long	get_time(void)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((long)current.tv_sec * 1000 + current.tv_usec / 1000);
}

void	print(char *str, char *color, t_philo *philo)
{
	printf("%s%ld %d %s%s", color, get_time() - philo->start_time, philo->id
		+ 1, str, RESET);
}

void	clean_exit(t_philo **philo)
{
	if ((*philo)->shared->forks_have_been_created)
	{
		sem_close((*philo)->shared->forks);
		sem_unlink(FSEM);
	}
	if ((*philo)->shared->waiter_sem_have_been_created)
	{
		sem_close((*philo)->shared->waiter);
		sem_unlink(WSEM);
	}
	if ((*philo)->shared->death_sem_have_been_created)
	{
		sem_close((*philo)->shared->death_sem);
		sem_unlink(DSEM);
	}
	if ((*philo)->shared->print_sem_have_been_created)
	{
		sem_close((*philo)->shared->print_sem);
		sem_unlink(PSEM);
	}
	if ((*philo)->shared->pid_tab_has_been_created)
		free((*philo)->shared->pid_tab);
	free((*philo)->shared);
	free((*philo));
}

int		wait_child(t_philo **philosopher)
{
	t_philo		*philo;
	int			status;
	int			i;
	
	philo = *philosopher;
	i = 0;
	while (i < philo->total_philos)
	{
		waitpid(philo->shared->pid_tab[i], &status, 0);
		i++;
	}
	return (1);
}