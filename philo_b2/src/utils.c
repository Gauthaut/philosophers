/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:01:13 by gaperaud          #+#    #+#             */
/*   Updated: 2025/01/02 04:46:55 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	check_input(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	if (ac != 5 && ac != 6)
		return (printf("input error\n"), exit(1));
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9' || j > 11)
				return (printf("input error\n"), exit(1));
			j++;
		}
		i++;
	}
	i = 1;
	while (av[i])
	{
		if (ft_atol(av[i]) == 0)
			return (printf("input error\n"), exit(1));
		i++;
	}
}

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

void	ft_usleep(long time, t_philo *philo)
{
	long	start;

	start = get_time();
	while (get_time() - start < time)
	{
		if (get_time() - philo->last_meal_time > philo->time_to_die)
		{
			print(DEAD, RED, philo);
			sem_wait(philo->print);
			sem_close(philo->forks);
			sem_close(philo->print);
			sem_close(philo->waiter);
			free(philo->pid_tab);
			exit(1);
		}
		usleep(100);
	}
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	print(char *str, char *color, t_philo *philo)
{
	sem_wait(philo->print);
	printf("%s%ld %d %s%s", color, get_time() - philo->start_time, philo->id
		+ 1, str, RESET);
	sem_post(philo->print);
}
