/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/15 13:00:45 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	cant_init_philo(int ac, char **av, t_philo **philosophers)
{
	*philosophers = malloc(sizeof(t_philo));
	if (!philosophers)
		return (true);
	(*philosophers)->id = 0;
	(*philosophers)->total_philos = ft_atoi(av[1]);
	(*philosophers)->time_to_die = ft_atoi(av[2]);
	(*philosophers)->time_to_eat = ft_atoi(av[3]);
	(*philosophers)->time_to_sleep = ft_atoi(av[4]);
	(*philosophers)->number_of_meal = -1;
	if (av[5])
		(*philosophers)->number_of_meal = ft_atoi(av[5]);
	(*philosophers)->shared = malloc(sizeof(t_shared));
	if (!(*philosophers)->shared)
		return (free(philosophers), true);
	(*philosophers)->shared->forks_have_been_created = 0;
	(*philosophers)->shared->print_sem_have_been_created = 0;
	(*philosophers)->shared->death_sem_have_been_created = 0;
	(*philosophers)->shared->waiter_sem_have_been_created = 0;
	(*philosophers)->shared->pid_tab_has_been_created = 0;
	(*philosophers)->shared->time_eaten = 0;
	(*philosophers)->shared->simulation_must_stop = 0;
	if (cant_init_semaphore(philosophers))
		return (clean_exit(philosophers), true);
	return (false);
}

void	exec_routine(t_philo *philo)
{
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
	clean_exit(&philo);
}

bool	cant_run_simulation(t_philo **philo)
{
	long	start;
	int		i;

	i = 0;
	start = get_time();
	(*philo)->start_time = start;
	(*philo)->last_meal = start;
	while (i++ < (*philo)->total_philos)
	{
		(*philo)->shared->pid_tab[i] = fork();
		if ((*philo)->shared->pid_tab < 0)
		{
			printf("fork error\n");
			sem_wait((*philo)->shared->death_sem);
			(*philo)->shared->simulation_must_stop = 1;
			sem_post((*philo)->shared->death_sem);
			return (clean_exit(philo), true);
		}
		if ((*philo)->shared->pid_tab[i] == 0)
		{
			(*philo)->id = i;
			exec_routine(*philo);
		}
	}
	return (false);
}

bool	monitor(t_philo *philo)
{
	long	current_time;

	current_time = get_time();
	if (philo->shared->simulation_must_stop)
		return (false);
	if (current_time - philo->last_meal > philo->time_to_die
		|| philo->shared->time_eaten >= philo->number_of_meal
		* philo->total_philos)
	{
		sem_wait(philo->shared->death_sem);
		philo->shared->simulation_must_stop = 1;
		sem_post(philo->shared->death_sem);
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	t_philo	*philosophers;
	int		i;

	i = 0;
	if (arguments_are_not_valid(ac, av))
		return (printf(RED "arguments not valids\n" RESET));
	if (cant_init_philo(ac, av, &philosophers))
		return (printf(RED "can't init philos 💀\n" RESET));
	if (cant_run_simulation(&philosophers))
		return (1);
	return (wait_child(&philosophers));
}
