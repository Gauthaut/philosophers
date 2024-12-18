/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:12 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/18 09:07:34 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

bool	cant_init_philo(t_philo *philosophers, int ac, char **av)
{
	if (arguments_are_not_valid(ac, av))
		return (printf(RED "arguments not valids\n" RESET));
	init_philo(philosophers, av);
	if (cant_init_semaphore(philosophers))
		return (true);
	return (false);
}

// le probleme ici est que le semaphore, simulation must stop bug.
// donc je ne vais pas l utiliser.
// la deuxieme solution est de faire en sorte que tout les processus
// s'arretent en meme temps, et d'eux meme.
// pour cela, lors de la mort d'un philosophe, on va envoyer un signal
// en creant un thread qui va wait le print semaphore, qui est binaire.
// Tout les philos vont bloques, et comme chaqe philo un moniteur qui
// check automatiquement si le philo est mort, on va pouvoir arreter
// tout les philos de cette maniere.

void	*monitordd(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	sem_wait(philo->print);
	return (NULL);
}

void	*monitord(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	sem_wait(philo->print);
	print(DEAD, RED, philo);
	sem_post(philo->print);
	return (NULL);
}

void	*monitor(void *args)
{
	t_philo	*philo;
	long	last_meal;

	philo = (t_philo *)args;
	while (1)
	{
		sem_wait(philo->child_monitor[philo->id]);
		last_meal = get_time() - philo->last_meal_time;
		sem_post(philo->child_monitor[philo->id]);
		// printf("last_meal: %ld\n", last_meal);
		if (last_meal > philo->time_to_die)
		{
			pthread_create(&philo->monitor_threadd, NULL, monitord, philo);
			pthread_detach(philo->monitor_threadd);
			// pthread_create(&philo->monitor_threaddd, NULL, monitordd, philo);
			// pthread_detach(philo->monitor_threaddd);
			sem_wait(philo->child_monitor[philo->id]);
			philo->child_must_stop = 1;
			sem_post(philo->child_monitor[philo->id]);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}

void	exec_child(t_philo *philo)
{
	pthread_create(&philo->monitor_thread, NULL, monitor, philo);
	if (philo->id % 2)
		usleep(200);
	while (1)
	{
		// printf("ate\n");
		if (philo_cant_eat(philo))
			break ;
		// printf("slept\n");
		if (philo_cant_sleep(philo))
			break ;
		// printf("thought\n");
		if (philo_cant_think(philo))
			break ;
	}
	pthread_join(philo->monitor_thread, NULL); // Attendre fin du monitor
	printf("ok2\n");
	exit(0);
}

bool	cant_run_philo(t_philo *philo)
{
	pid_t	*pid_tab;
	int		i;

	pid_tab = malloc(sizeof(pid_t) * (philo->total_philos));
	if (!pid_tab)
		return (free((*philo).child_monitor), true);
	i = -1;
	memset(pid_tab, 0, sizeof(pid_t) * (*philo).total_philos);
	(*philo).start_time = get_time();
	(*philo).last_meal_time = (*philo).start_time;
	while (++i < (*philo).total_philos)
	{
		(*philo).id = i;
		pid_tab[i] = fork();
		if (pid_tab[i] < 0)
			return (stop_simulation(philo, pid_tab, 1), true);
		if (pid_tab[i] == 0)
			exec_child(philo);
	}
	if (cant_run_meal_monitor(philo))
		return (stop_simulation(philo, pid_tab, 1), true);
	stop_simulation(philo, pid_tab, 0);
	return (false);
}

int	main(int ac, char **av)
{
	t_philo	philosophers;

	if (cant_init_philo(&philosophers, ac, av))
		return (printf(RED "can't init philos\n" RESET));
	if (cant_run_philo(&philosophers))
		return (1);
	return (0);
}
