/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legoat <legoat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 02:45:00 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/03 04:45:16 by legoat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	unlink_sem(int last_index)
{
	char	*str;
	int		i;

	i = 0;
	sem_unlink(FSEM);
	sem_unlink(PSEM);
	sem_unlink(SSEM);
	sem_unlink(WSEM);
	sem_unlink(MSEM);
	while (i < last_index)
	{
		str = get_sem_name('/', i);
		sem_unlink(str);
		i++;
	}
}

void	close_sem(t_philo *philo, int last_index)
{
	int		i;

	i = 0;
	sem_close(philo->forks);
	sem_close(philo->print);
	sem_close(philo->stop_simulation_sem);
	sem_close(philo->waiter);
	if (philo->number_of_meal != -1)
		sem_close(philo->meal_counter);
	while (i < last_index)
	{
		sem_close(philo->child_monitor[i]);
		i++;
	}
}

void	stop_simulation(t_philo *philo, pid_t *pid_tab)
{
	int	i;

	sem_wait(philo->stop_simulation_sem);
	i = 0;
	while (i < (*philo).total_philos && pid_tab[i] != 0)
	{
		kill(pid_tab[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < (*philo).total_philos && pid_tab[i] != 0)
	{
		waitpid(pid_tab[i], NULL, 0);
		i++;
	}
	close_sem(philo, philo->total_philos);
	unlink_sem(philo->total_philos);
	free(philo->child_monitor);
	free(pid_tab);
}

void	*meal_monitor(void *args)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)args;
	i = 0;
	while (i < philo->total_philos)
	{
		sem_wait(philo->meal_counter);
		i++;
	}
	sem_post(philo->stop_simulation_sem);
	return (NULL);
}

bool	cant_run_meal_monitor(t_philo *philo)
{
	if (philo->number_of_meal == -1)
		return (false);
	if (pthread_create(&philo->meal_monitor, NULL, meal_monitor, philo) != 0)
		return (true);
	pthread_detach(philo->meal_monitor);
	return (false);
}
