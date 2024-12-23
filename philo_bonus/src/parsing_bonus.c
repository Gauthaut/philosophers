/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:01:15 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/20 23:13:02 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	arguments_are_not_valid(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	if ((ac != 5 && ac != 6) || ft_atoi(av[1]) == 0)
		return (true);
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (true);
			j++;
		}
		if (ft_strlen(av[i]) > 11)
			return (false);
		i++;
	}
	return (false);
}

void	init_philo(t_philo *philo, char **av)
{
	(*philo).total_philos = ft_atoi(av[1]);
	(*philo).time_to_die = ft_atoi(av[2]);
	(*philo).time_to_eat = ft_atoi(av[3]) * 1000;
	(*philo).time_to_sleep = ft_atoi(av[4]) * 1000;
	(*philo).time_to_think = 0;
	if ((*philo).total_philos % 2)
		(*philo).time_to_think = (*philo).time_to_eat * 0.9;
	(*philo).number_of_meal = -1;
	if (av[5])
		(*philo).number_of_meal = ft_atoi(av[5]);
	(*philo).time_eaten = 0;
	(*philo).child_must_stop = 0;
}

char	*get_sem_name(char type, int index)
{
	static char	str[6] = {0};
	int			i;

	i = 1;
	if (index == 0)
	{
		memset(str, 'A', sizeof(char) * 5);
		str[0] = type;
		str[5] = 0;
	}
	else
	{
		while (str[i] == 'Z')
		{
			str[i] = 'A';
			i++;
		}
		str[i]++;
	}
	return (str);
}

bool	cant_init_semaphore(t_philo *philo)
{
	int	i;

	unlink_sem(philo->total_philos);
	(*philo).child_monitor = malloc(sizeof(sem_t *) * (*philo).total_philos);
	if (!(*philo).child_monitor)
		return (true);
	i = -1;
	while (++i < (*philo).total_philos)
	{
		(*philo).child_monitor[i] = sem_open(get_sem_name('/', i),
				O_CREAT, 0644, 1);
	}
	(*philo).waiter = sem_open(WSEM, O_CREAT, 0666, (*philo).total_philos / 2);
	(*philo).forks = sem_open(FSEM, O_CREAT, 0644, (*philo).total_philos);
	(*philo).print = sem_open(PSEM, O_CREAT, 0644, 1);
	if (philo->number_of_meal != -1)
		(*philo).meal_counter = sem_open(MSEM, O_CREAT, 0644, 0);
	return (false);
}
