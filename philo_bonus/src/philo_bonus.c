/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/23 08:00:57 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	cant_init_philo(t_philo *philosophers, int ac, char **av)
{
	if (arguments_are_not_valid(ac, av))
		return (printf(RED "arguments not valids\n" RESET));
	init_philo(philosophers, ac, av);
	if (cant_init_semaphore(philosophers))
		return (true);
	return (false);
}

void	*monitor(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		if (philo_is_dead(philo))
			sem_post(philo->stop_simulation_sem);
		if (philo_ate_enough(philo))
			break ;
		usleep(1000);
	}
	printf("monitor %d \n", philo->id);
	sem_wait(philo->child_monitor[philo->id]);
	philo->child_must_stop = 1;
	sem_post(philo->child_monitor[philo->id]);
	return (NULL);
}

void	exec_child(t_philo *philo)
{
	pthread_create(&philo->monitor_thread, NULL, monitor, philo);
	if (philo->id % 2)
		usleep(500);
	while (1)
	{
		if (philo_cant_eat(philo))
			break ;
		if (philo_cant_sleep(philo))
			break ;
		if (philo_cant_think(philo))
			break ;
	}
	pthread_join(philo->monitor_thread, NULL);
	exit(0);
}

bool	cant_run_philo(t_philo *philo)
{
	pid_t	*pid_tab;
	int		i;

	pid_tab = malloc(sizeof(pid_t) * (philo->total_philos));
	if (!pid_tab)
		return (free((*philo).child_monitor), free((*philo).waiter), true);
	i = 0;
	memset(pid_tab, 0, sizeof(pid_t) * (*philo).total_philos);
	(*philo).start_time = get_time();
	(*philo).last_meal_time = (*philo).start_time;
	while (i < (*philo).total_philos)
	{
		(*philo).id = i;
		pid_tab[i] = fork();
		if (pid_tab[i] < 0)
			return (stop_simulation(philo, pid_tab), free(pid_tab), true);
		if (pid_tab[i] == 0)
			exec_child(philo);
		else
			i++;
	}
	stop_simulation(philo, pid_tab);
	free(pid_tab);
	return (false);
}

int	main(int ac, char **av)
{
	t_philo philosophers;
	int i;

	i = 0;
	if (cant_init_philo(&philosophers, ac, av))
		return (printf(RED "can't init philos\n" RESET));
	if (cant_run_philo(&philosophers))
		return (1);
	return (0);
}