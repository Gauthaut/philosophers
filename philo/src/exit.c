/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:56:29 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/16 22:53:23 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_usleep(int time_to_usleep, t_philo *philo)
{
	long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) * 1000 < time_to_usleep)
	{
		pthread_mutex_lock(&philo->ressources->stop_mutex);
		if (philo_is_dead(philo))
		{
			philo->is_dead = 1;
			pthread_mutex_unlock(&philo->ressources->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->ressources->stop_mutex);
		usleep(50);
	}
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	if (philo_is_dead(philo))
		philo->is_dead = 1;
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
}

void	free_philo(t_philo *philo)
{
	if (philo->id != philo->total_philo - 1 && philo->next)
		free_philo(philo->next);
	free(philo);
}

void	join_threads(t_philo *philosophers, int last_id)
{
	t_philo	*philo;

	philo = philosophers;
	while (1)
	{
		pthread_join(philo->philo, NULL);
		if (philo->id == last_id - 1)
			break ;
		philo = philo->next;
	}
}

void	destroy_mutexes(t_philo *philo, int last_id)
{
	if (philo->ressources->print_mutex_is_initialised)
		pthread_mutex_destroy(&philo->ressources->print_mutex);
	if (philo->ressources->stop_mutex_is_initialised)
		pthread_mutex_destroy(&philo->ressources->stop_mutex);
	if (philo->ressources->eat_mutex_is_initialised)
		pthread_mutex_destroy(&philo->ressources->eat_mutex);
	while (1)
	{
		pthread_mutex_destroy(&philo->fork);
		if (philo->id == last_id - 1)
			break ;
		philo = philo->next;
	}
}

void	clean_exit(t_philo *philosophers, int last_id, int flag)
{
	if (flag == 2)
		join_threads(philosophers, last_id);
	if (flag >= 1)
		destroy_mutexes(philosophers, last_id);
	if (philosophers)
		free_philo(philosophers);
}
