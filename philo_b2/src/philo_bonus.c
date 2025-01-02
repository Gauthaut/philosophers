/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2025/01/02 04:46:25 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	exit_philo(t_philo *philo)
{
	int	i;

	i = 0;
	while (philo->pid_tab && philo->pid_tab[i++] != 0)
		kill(philo->pid_tab[philo->id], SIGKILL);
	if (philo->pid_tab)
		free(philo->pid_tab);
	if (philo->forks)
	{
		sem_close(philo->forks);
		sem_unlink(FSEM);
	}
	if (philo->print)
	{
		sem_close(philo->print);
		sem_unlink(PSEM);
	}
	if (philo->waiter)
	{
		sem_close(philo->waiter);
		sem_unlink(WSEM);
	}
	exit(philo->exit_value);
}

void	init_philo(t_philo *philo, int ac, char **av)
{
	check_input(ac, av);
	philo->total_philos = ft_atol(av[1]);
	philo->time_to_die = ft_atol(av[2]);
	philo->time_to_eat = ft_atol(av[3]);
	philo->time_to_sleep = ft_atol(av[4]);
	philo->number_of_meal = -1;
	if (ac == 6)
		philo->number_of_meal = ft_atol(av[5]);
	sem_unlink(WSEM);
	philo->waiter = sem_open(WSEM, O_CREAT, 0666, philo->total_philos / 2);
	sem_unlink(FSEM);
	philo->forks = sem_open(FSEM, O_CREAT, 0666, philo->total_philos);
	sem_unlink(PSEM);
	philo->print = sem_open(PSEM, O_CREAT, 0666, 1);
	philo->pid_tab = NULL;
	philo->exit_value = 0;
	if (philo->waiter == SEM_FAILED || philo->forks == SEM_FAILED
		|| philo->print == SEM_FAILED)
		exit_philo(philo);
	philo->pid_tab = malloc(sizeof(pid_t) * (philo->total_philos + 1));
	if (!philo->pid_tab)
		exit_philo(philo);
}

void	exec_child(t_philo *philo)
{
	while (philo->number_of_meal)
	{
		sem_wait(philo->waiter);
		sem_wait(philo->forks);
		print(FORK, GREY, philo);
		sem_wait(philo->forks);
		print(FORK, GREY, philo);
		philo->last_meal_time = get_time();
		print(EAT, YELLOW, philo);
		ft_usleep(philo->time_to_eat, philo);
		sem_post(philo->forks);
		sem_post(philo->forks);
		sem_post(philo->waiter);
		philo->number_of_meal--;
		print(SLEEP, BLUE, philo);
		ft_usleep(philo->time_to_sleep, philo);
		print(THINK, GREEN, philo);
		if (philo->total_philos % 2)
			ft_usleep((philo->time_to_eat * 0.9), philo);
	}
	sem_close(philo->forks);
	sem_close(philo->print);
	sem_close(philo->waiter);
	free(philo->pid_tab);
	exit(0);
}

void	run_philo(t_philo *philo)
{
	int	status;
	int	i;

	i = -1;
	philo->start_time = get_time();
	memset(philo->pid_tab, 0, sizeof(pid_t) * (philo->total_philos + 1));
	while (++i < philo->total_philos)
	{
		philo->id = i;
		philo->pid_tab[i] = fork();
		if (philo->pid_tab[i] == -1)
			exit_philo(philo);
		if (philo->pid_tab[i] == 0)
			exec_child(philo);
	}
	i = -1;
	status = 0;
	while (!status && ++i < philo->total_philos)
		waitpid(-1, &status, 0);
	philo->exit_value = status;
	exit_philo(philo);
}

int	main(int ac, char **av)
{
	t_philo	philosophers;

	init_philo(&philosophers, ac, av);
	run_philo(&philosophers);
	return (0);
}
