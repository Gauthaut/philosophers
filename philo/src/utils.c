/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 02:30:45 by gaperaud          #+#    #+#             */
/*   Updated: 2024/09/25 07:53:21 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atoi(char *str)
{
	int	num;
	int	i;

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
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	if (philo->ressources->simulation_must_stop)
	{
		pthread_mutex_unlock(&philo->ressources->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	pthread_mutex_lock(&philo->ressources->print_mutex);
	printf("%s%ld %d %s%s", color, get_time() - philo->start_time, philo->id
		+ 1, str, RESET);
	pthread_mutex_unlock(&philo->ressources->print_mutex);
}