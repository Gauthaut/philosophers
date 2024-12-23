/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/23 09:22:01 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	cant_init_philo(t_philo *philosophers, int ac, char **av)
{
	if (arguments_are_not_valid(ac, av))
		return (printf(RED "arguments not valids\n" RESET));
	init_philo(philosophers, av);
	if (cant_init_semaphore(philosophers))
		return (true);
	return (false);
}

void	*monitor(void *args)
{
	t_philo	*philo;
	long	last_meal;

	philo = (t_philo *)args;
	while (1)
	{
		sem_wait(philo->child_monitor[philo->id]);
		last_meal = get_time() - philo->last_meal_time;
		sem_post(philo->child_monitor[philo->id]);
		if (last_meal > philo->time_to_die)
		{
			sem_wait(philo->print);
			print(DEAD, RED, philo);
			sem_post(philo->print);
			philo->child_must_stop = 1;
			exit (1);
		}
		usleep(1000);
	}
	return (NULL);
}

void	exec_child(t_philo *philo)
{
	pthread_create(&philo->monitor_thread, NULL, monitor, philo);
	pthread_detach(philo->monitor_thread);
	if (philo->id % 2)
		usleep(200);
	while (!philo->child_must_stop)
	{
		if (philo_cant_eat(philo))
			break ;
		if (philo_cant_sleep(philo))
			break ;
		if (philo_cant_think(philo))
			break ;
	}
	printf("child %d stopped\n", philo->id);
	exit(1);
}

bool	cant_run_philo(t_philo *philo)
{
	pid_t	*pid_tab;
	int		i;
	int		status = 0;

	pid_tab = malloc(sizeof(pid_t) * (philo->total_philos));
	if (!pid_tab)
		return (free((*philo).child_monitor), true);
	i = -1;
	(*philo).start_time = get_time();
	(*philo).last_meal_time = (*philo).start_time;
	while (++i < (*philo).total_philos)
	{
		(*philo).id = i;
		pid_tab[i] = fork();
		if (pid_tab[i] < 0)
			return (stop_simulation(philo, pid_tab), true);
		if (pid_tab[i] == 0)
			exec_child(philo);
	}
	if (cant_run_meal_monitor(philo))
		return (stop_simulation(philo, pid_tab), true);
	while (!status)
		waitpid(-1, &status, 0);
	stop_simulation(philo, pid_tab);
	return (false);
}

int	main(int ac, char **av)
{
	t_philo	philosophers;

	if (cant_init_philo(&philosophers, ac, av))
		return (printf(RED "can't init philos\n" RESET));
	if (cant_run_philo(&philosophers))
		return (1);
	return (0);
}
