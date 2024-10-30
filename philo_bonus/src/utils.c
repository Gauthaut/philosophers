/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 02:30:45 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/30 21:58:42 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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

int	ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

long	get_time(void)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((long)current.tv_sec * 1000 + current.tv_usec / 1000);
}

void	print(char *str, char *color, t_philo *philo)
{
	printf("%s%ld %d %s%s", color, get_time() - philo->start_time, philo->id
		+ 1, str, RESET);
}
