/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 02:30:45 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/14 08:46:13 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atol(char *str)
{
	long	num;
	long	i;

	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num);
}

long	get_time(void)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((long)current.tv_sec * 1000 + current.tv_usec / 1000);
}

void	print(char *str, char *color, t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->ressources->stop_mutex);
	res = philo->ressources->simulation_must_stop;
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	if (res)
		return ;
	pthread_mutex_lock(&philo->ressources->print_mutex);
	printf("%s%ld %d %s%s", color, get_time() - philo->start_time, philo->id
		+ 1, str, RESET);
	pthread_mutex_unlock(&philo->ressources->print_mutex);
}

bool	philo_is_dead(t_philo *philo)
{
	int	time_since_last_meal;

	pthread_mutex_lock(&philo->ressources->eat_mutex);
	time_since_last_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->ressources->eat_mutex);
	if (time_since_last_meal > philo->time_to_die)
	{
		philo->is_dead = 1;
		return (true);
	}
	return (false);
}

bool	philo_ate_enough(t_philo *philo)
{
	int	time_eaten;

	if (philo->number_of_meal == -1)
		return (false);
	pthread_mutex_lock(&philo->ressources->eat_mutex);
	time_eaten = philo->ressources->time_eaten;
	pthread_mutex_unlock(&philo->ressources->eat_mutex);
	if (time_eaten >= philo->number_of_meal * philo->total_philo)
		return (true);
	return (false);
}
