/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 03:26:35 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/17 02:34:38 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(&philo->fork);
		print(FORK, GREY, philo);
		pthread_mutex_lock(&philo->next->fork);
		print(FORK, GREY, philo);
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork);
		print(FORK, GREY, philo);
		pthread_mutex_lock(&philo->fork);
		print(FORK, GREY, philo);
	}
	print(EAT, YELLOW, philo);
}

void	drop_the_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_unlock(&philo->next->fork);
		pthread_mutex_unlock(&philo->fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
	}
}

bool	philo_cant_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	if (philo->ressources->simulation_must_stop)
	{
		pthread_mutex_unlock(&philo->ressources->stop_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	if (philo->id == philo->next->id)
	{
		print(FORK, GREY, philo);
		ft_usleep(philo->time_to_die * 1000, philo);
		return (true);
	}
	take_forks(philo);
	pthread_mutex_lock(&philo->ressources->eat_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->ressources->eat_mutex);
	ft_usleep(philo->time_to_eat * 1000, philo);
	drop_the_forks(philo);
	pthread_mutex_lock(&philo->ressources->eat_mutex);
	philo->ressources->time_eaten++;
	pthread_mutex_unlock(&philo->ressources->eat_mutex);
	return (false);
}

bool	philo_cant_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	if (philo->ressources->simulation_must_stop)
	{
		pthread_mutex_unlock(&philo->ressources->stop_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	print(SLEEP, BLUE, philo);
	ft_usleep(philo->time_to_sleep * 1000, philo);
	return (false);
}

bool	philo_cant_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	if (philo->ressources->simulation_must_stop)
	{
		pthread_mutex_unlock(&philo->ressources->stop_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	print(THINK, GREEN, philo);
	return (false);
}
