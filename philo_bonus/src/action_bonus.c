/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:41:09 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/23 05:57:19 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// void	take_fork(t_philo *philo)
// {
// 	if (philo->id % 2)
// 	{
// 		sem_wait(philo->waiter[philo->id % philo->total_philos]);
// 		print(FORK, GREY, philo);
// 		sem_wait(philo->waiter[(philo->id + 1) % philo->total_philos]);
// 		print(FORK, GREY, philo);
// 	}
// 	else
// 	{
// 		sem_wait(philo->waiter[(philo->id + 1) % philo->total_philos]);
// 		print(FORK, GREY, philo);
// 		sem_wait(philo->waiter[philo->id % philo->total_philos]);
// 		print(FORK, GREY, philo);
// 	}
// 	sem_wait(philo->child_monitor[philo->id]);
// 	philo->last_meal_time = get_time ();
// 	sem_post(philo->child_monitor[philo->id]);
// 	sem_wait(philo->print);
// 	print(EAT, YELLOW, philo);
// 	sem_post(philo->print);
// }

// void	drop_the_fork(t_philo *philo)
// {
// 	if (philo->id % 2)
// 	{
// 		sem_post(philo->waiter[(philo->id + 1) % philo->total_philos]);
// 		sem_post(philo->waiter[philo->id % philo->total_philos]);
// 	}
// 	else
// 	{
// 		sem_wait(philo->waiter[philo->id % philo->total_philos]);
// 		sem_wait(philo->waiter[(philo->id + 1) % philo->total_philos]);
// 	}
// }

bool	philo_cant_eat(t_philo *philo)
{
	sem_wait(philo->child_monitor[philo->id]);
	if (philo->child_must_stop)
	{
		sem_post(philo->child_monitor[philo->id]);
		return (false);
	}
	sem_post(philo->child_monitor[philo->id]);
	// take_fork(philo);
	sem_wait(philo->forks);
	sem_wait(philo->print);
	print(FORK, GREY, philo);
	sem_post(philo->print);
	sem_wait(philo->forks);
	sem_wait(philo->print);
	print(EAT, GREY, philo);
	print(EAT, YELLOW, philo);
	sem_post(philo->print);
	usleep(philo->time_to_eat);
	sem_post(philo->forks);
	sem_post(philo->forks);
	// drop_the_fork(philo);
	return (false);
}

bool	philo_cant_sleep(t_philo *philo)
{
	sem_wait(philo->child_monitor[philo->id]);
	if (philo->child_must_stop)
	{
		sem_post(philo->child_monitor[philo->id]);
		return (true);
	}
	sem_post(philo->child_monitor[philo->id]);
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