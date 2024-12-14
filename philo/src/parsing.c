/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:01:15 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/14 08:53:09 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	arguments_are_not_valid(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	if ((ac != 5 && ac != 6) || ft_atol(av[1]) == 0)
		return (true);
	while (av[i])
	{
		if (atol(av[i]) > 2147483647 || atol(av[i]) < -2147483648)
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (true);
			j++;
		}
		i++;
	}
	return (false);
}

t_shared_ressources	*create_shared_ressources(void)
{
	t_shared_ressources	*r;

	r = malloc(sizeof(t_shared_ressources));
	if (!r)
		return (NULL);
	r->print_mutex_is_initialised = 0;
	r->stop_mutex_is_initialised = 0;
	r->eat_mutex_is_initialised = 0;
	r->simulation_must_stop = 0;
	r->time_eaten = 0;
	return (r);
}

t_philo	*new_philo(char **av, int i, t_shared_ressources *r)
{
	t_philo	*philo;

	philo = malloc(sizeof(*philo));
	if (!philo)
		return (NULL);
	philo->id = i;
	philo->total_philo = ft_atol(av[1]);
	philo->time_to_die = ft_atol(av[2]);
	philo->time_to_eat = ft_atol(av[3]);
	philo->time_to_sleep = ft_atol(av[4]);
	philo->number_of_meal = -1;
	if (av[5])
		philo->number_of_meal = ft_atol(av[5]);
	philo->time_to_think = 0;
	if (philo->total_philo % 2)
		philo->time_to_think = philo->time_to_die * 0.9;
	philo->is_dead = 0;
	philo->next = NULL;
	philo->ressources = r;
	return (philo);
}

void	add_back_philo(t_philo **philo, t_philo *new)
{
	t_philo	*temp;

	if (*philo == NULL)
	{
		*philo = new;
		return ;
	}
	temp = *philo;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

bool	cant_init_mutex(t_philo **philosophers)
{
	t_philo	*philo;

	philo = *philosophers;
	if (pthread_mutex_init(&philo->ressources->print_mutex, NULL) != 0)
		return (clean_exit(*philosophers, 0, 0), true);
	philo->ressources->print_mutex_is_initialised = 1;
	if (pthread_mutex_init(&philo->ressources->stop_mutex, NULL) != 0)
		return (clean_exit(*philosophers, 0, 1), true);
	philo->ressources->stop_mutex_is_initialised = 1;
	if (pthread_mutex_init(&philo->ressources->eat_mutex, NULL) != 0)
		return (clean_exit(*philosophers, 0, 1), true);
	philo->ressources->eat_mutex_is_initialised = 1;
	while (1)
	{
		if (pthread_mutex_init(&philo->fork, NULL) != 0)
			return (clean_exit(*philosophers, philo->id, 1), true);
		if (philo->id == philo->total_philo - 1)
			break ;
		philo = philo->next;
	}
	return (false);
}
