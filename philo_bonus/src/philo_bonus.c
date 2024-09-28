/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 05:25:10 by gaperaud          #+#    #+#             */
/*   Updated: 2024/09/27 05:32:04 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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

int main (int ac, char **av)
{
	t_philo		*philosophers;

	if (cant_init_philo(ac, av, &philosophers))
		return (printf("initialisation error\n"));
	if (cant_run_philo(&philosophers))
		return (1);
	return (0);
}