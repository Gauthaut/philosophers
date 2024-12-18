/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:41:09 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/18 08:33:51 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	philo_cant_eat(t_philo *philo)
{
	printf("here1\n");
	sem_wait(philo->child_monitor[philo->id]);
	if (philo->child_must_stop)
	{
		printf("here2\n");
		sem_post(philo->child_monitor[philo->id]);
		return (false);
	}
	printf("here3\n");
	sem_post(philo->child_monitor[philo->id]);
	printf("heeeere\n"); // dans le cas de 1 philo, il ne peut pas manger et le mon fait sortir ici
	sem_wait(philo->waiter);
	printf("heeeere\n"); // dans le cas de 1 philo, il ne peut pas manger et le mon fait sortir ici
	sem_wait(philo->forks);
	sem_wait(philo->print);
	print(FORK, GREY, philo);
	sem_post(philo->print);
	(sem_wait(philo->forks), sem_wait(philo->print));
	(print(FORK, GREY, philo), print(EAT, YELLOW, philo));
	sem_post(philo->print);
	sem_wait(philo->child_monitor[philo->id]);
	philo->last_meal_time = get_time();
	sem_post(philo->child_monitor[philo->id]);
	usleep(philo->time_to_eat);
	(sem_post(philo->forks), sem_post(philo->forks));
	sem_post(philo->waiter);
	philo->time_eaten++;
	if (philo->time_eaten == philo->number_of_meal)
		sem_post(philo->meal_counter);
	return (false);
}

bool	philo_cant_sleep(t_philo *philo)
{
	printf("here4\n");
	sem_wait(philo->child_monitor[philo->id]);
	if (philo->child_must_stop)
	{
		printf("here5\n");
		sem_post(philo->child_monitor[philo->id]);
		return (true);
	}
	printf("here6\n");
	sem_post(philo->child_monitor[philo->id]);
	sem_wait(philo->print);
	print(SLEEP, BLUE, philo);
	sem_post(philo->print);
	usleep(philo->time_to_sleep);
	return (false);
}

bool	philo_cant_think(t_philo *philo)
{
	sem_wait(philo->child_monitor[philo->id]);
	if (philo->child_must_stop)
	{
		sem_post(philo->child_monitor[philo->id]);
		return (true);
	}
	sem_post(philo->child_monitor[philo->id]);
	sem_wait(philo->print);
	print(THINK, GREEN, philo);
	sem_post(philo->print);
	if (philo->total_philos % 2)
		usleep(philo->time_to_think);
	return (false);
}
