/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2024/09/27 00:06:58 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	cant_init_philo(int ac, char **av, t_philo **philosophers)
{
	t_shared_ressources	*r;
	t_philo				*temp;
	int					i;

	if (arguments_are_not_valid(ac, av))
		return (true);
	r = create_shared_ressources();
	if (!r)
		return (true);
	i = -1;
	*philosophers = NULL;
	while (++i < ft_atoi(av[1]))
	{
		temp = new_philo(av, i);
		if (!temp)
			return (clean_exit(*philosophers, 0, 0), true);
		temp->ressources = r;
		add_back_philo(philosophers, temp);
	}
	temp->next = *philosophers;
	if (cant_init_mutex(philosophers))
		return (printf(RED "%s" RESET, MERROR), true);
	return (false);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2)
		usleep(500);
	while (1)
	{
		if (philo_cant_eat(philo))
			break ;
		if (philo_cant_sleep(philo))
			break ;
		if (philo_cant_think(philo))
			break ;
	}
	return (NULL);
}

bool	cant_run_simulation(t_philo **philosophers)
{
	t_philo	*philo;
	long	start;

	philo = *philosophers;
	start = get_time();
	while (1)
	{
		philo->start_time = start;
		philo->last_meal = start;
		if (pthread_create(&philo->philo, NULL, routine, philo) != 0)
		{
			printf("thread initialisation error\n");
			return (clean_exit(*philosophers, philo->id, 2), true);
		}
		if (philo->id == philo->total_philo - 1)
			return (false);
		philo = philo->next;
	}
}

void	monitor(t_philo **philosophers)
{
	t_philo(*philo) = *philosophers;
	long(magic_count);
	while (1)
	{
		pthread_mutex_lock(&philo->ressources->eat_mutex);
		magic_count = get_time() - philo->last_meal;
		//printf("currentime = %ld, timeto die = %ld\n", magic_count, philo->time_to_die);
		pthread_mutex_unlock(&philo->ressources->eat_mutex);
		if (magic_count > philo->time_to_die)
			break ;
		pthread_mutex_lock(&philo->ressources->eat_mutex);
		magic_count = philo->ressources->time_eaten;
		pthread_mutex_unlock(&philo->ressources->eat_mutex);
		if (philo->number_of_meal * philo->total_philo <= magic_count)
			break ;
		if (philo->id == philo->total_philo - 1)
			usleep(2000);
		philo = philo->next;
	}
	pthread_mutex_lock(&philo->ressources->stop_mutex);
	philo->ressources->simulation_must_stop = 1;
	pthread_mutex_unlock(&philo->ressources->stop_mutex);
	join_threads(*philosophers, philo->total_philo);
	if (philo->ressources->time_eaten != philo->number_of_meal)
		print(DEAD, RED, philo);
	clean_exit(*philosophers, philo->total_philo, 1);
}

int	main(int ac, char **av)
{
	t_philo *philosophers;

	if (cant_init_philo(ac, av, &philosophers))
		return (printf(RED "can't init philos 💀\n" RESET));
	if (cant_run_simulation(&philosophers))
		return (1);
	monitor(&philosophers);
	return (0);
}