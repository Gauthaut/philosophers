/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 02:45:00 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/23 04:55:05 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	destroy_sem(t_philo *philo, int last_index)
{
	char	*str;
	int		i;

	i = 0;
	sem_close(philo->forks);
	sem_unlink(FSEM);
	sem_close(philo->print);
	sem_unlink(PSEM);
	sem_close(philo->stop_simulation_sem);
	sem_unlink(SSEM);
	while (i < last_index)
	{
		str = get_sem_name(philo, 'C', i);
		sem_close(philo->child_monitor[i]);
		sem_unlink(str);
		i++;
	}
	i = 0;
	while (i < last_index)
	{
		str = get_sem_name(philo, 'W', i);
		sem_close(philo->waiter[i]);
		sem_unlink(str);
		i++;
	}
}

void	stop_simulation(t_philo *philo, pid_t *pid_tab)
{
	int	i;

	sem_wait(philo->stop_simulation_sem);
	printf("simulation will end\n");
	i = 0;
	while (i < (*philo).total_philos && (*philo).pid_tab[i] != 0)
	{
		kill(pid_tab[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < (*philo).total_philos && (*philo).pid_tab[i] != 0)
	{
		waitpid(pid_tab[i], NULL, 0);
		i++;
	}
	destroy_sem(philo, i);
	free(philo->waiter);
	free(philo->child_monitor);
}

bool	philo_is_dead(t_philo *philo)
{
	long	last_meal;

	sem_wait(philo->child_monitor[philo->id]);
	last_meal = get_time () - philo->last_meal_time;
	sem_post(philo->child_monitor[philo->id]);
	// printf("last meal p %d = %ld beat\n", philo->id, last_meal);
	if (last_meal > philo->time_to_die)
		return (true);
	return (false);
}

bool	philo_ate_enough(t_philo *philo)
{
	int time_eaten;

	if (philo->number_of_meal == -1)
		return (false);
	sem_wait(philo->child_monitor[philo->id]);
	time_eaten = philo->time_eaten;
	sem_post(philo->child_monitor[philo->id]);
	if (time_eaten >= philo->number_of_meal)
		return (true);
	return (false);
}