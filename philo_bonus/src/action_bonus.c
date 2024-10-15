/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:41:09 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/14 18:30:20 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	take_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		usleep(1000);
		sem_wait(philo->shared->forks);
		sem_wait(philo->shared->print_sem);
		print(FORK, GREY, philo);
		sem_post(philo->shared->print_sem);
		sem_wait(philo->shared->forks);
		sem_wait(philo->shared->print_sem);
		print(FORK, GREY, philo);
		sem_post(philo->shared->print_sem);
	}
	else
	{
		sem_wait(philo->shared->forks);
		sem_wait(philo->shared->print_sem);
		print(FORK, GREY, philo);
		sem_post(philo->shared->print_sem);
		sem_wait(philo->shared->forks);
		sem_wait(philo->shared->print_sem);
		print(FORK, GREY, philo);
		sem_post(philo->shared->print_sem);
	}
}

bool	philo_cant_eat(t_philo *philo)
{
	if (!monitor(philo))
		return (true);
	if (philo->total_philos == 1)
	{
		sem_wait(philo->shared->print_sem);
		print(FORK, GREY, philo);
		sem_post(philo->shared->print_sem);
		usleep(philo->time_to_eat * 1000);
		return (true);
	}
	sem_wait(philo->shared->waiter);
	take_fork(philo);
	sem_wait(philo->shared->print_sem);
	print(EAT, GREY, philo);
	sem_post(philo->shared->print_sem);
	usleep(philo->time_to_eat * 1000);
	sem_post(philo->shared->forks);
	sem_post(philo->shared->forks);
	sem_post(philo->shared->waiter);
	return (false);
}

bool	philo_cant_sleep(t_philo *philo)
{
	if (!monitor(philo))
		return (true);
	sem_wait(philo->shared->print_sem);
	print(SLEEP, BLUE, philo);
	sem_post(philo->shared->print_sem);
	usleep(philo->time_to_sleep * 1000);
	return (false);
}

bool	philo_cant_think(t_philo *philo)
{
	if (!monitor(philo))
		return (true);
	sem_wait(philo->shared->print_sem);
	print(THINK, GREEN, philo);
	sem_post(philo->shared->print_sem);
	if (philo->total_philos % 2)
		usleep(philo->time_to_eat * 0.9 * 1000);
	return (false);
}