/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:16 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/17 02:35:45 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\x1b[37m"
# define GREEN "\x1b[37m"
# define BLUE "\x1b[37m"
# define VIOLET "\x1b[37m"
# define GREY "\x1b[37m"
# define YELLOW "\x1b[37m"
# define RESET "\x1b[37m"

# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DEAD "%ld %d is dead\n"
# define MERROR "mutex initialisation error\n"

// # define FORK "has taken a fork 🍴\n"
// # define EAT "is eating 🍝\n"
// # define SLEEP "is sleeping 🌙\n"
// # define THINK "is thinking 🤔\n"
// # define DEAD "%ld %d is dead 💀\n"
// # define MERROR "mutex initialisation error\n"

// # define RED "\x1b[31m"
// # define GREEN "\x1b[32m"
// # define BLUE "\x1b[34m"
// # define VIOLET "\x1b[35m"
// # define GREY "\x1b[90m"
// # define YELLOW "\x1b[33m"
// # define RESET "\x1b[0m"

typedef struct s_philo
{
	int							total_philo;
	int							id;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	int							number_of_meal;
	long						last_meal;
	long						start_time;
	int							is_dead;
	pthread_t					philo;
	pthread_mutex_t				fork;
	struct s_shared_ressources	*ressources;
	struct s_philo				*next;
}								t_philo;

typedef struct s_shared_ressources
{
	pthread_mutex_t				print_mutex;
	pthread_mutex_t				stop_mutex;
	pthread_mutex_t				eat_mutex;
	int							print_mutex_is_initialised;
	int							stop_mutex_is_initialised;
	int							eat_mutex_is_initialised;
	int							simulation_must_stop;
	int							time_eaten;
}								t_shared_ressources;

/* PARSING */

bool							arguments_are_not_valid(int ac, char **av);
t_shared_ressources				*create_shared_ressources(void);
t_philo							*new_philo(char **av, int i,
									t_shared_ressources *r);
void							add_back_philo(t_philo **philo, t_philo *new);
bool							cant_init_mutex(t_philo **philosophers);

/* EXIT */

void							free_philo(t_philo *philo);
void							join_threads(t_philo *philo, int last_id);
void							destroy_mutexes(t_philo *philo, int last_id);
void							clean_exit(t_philo *philos, int last_id, int f);
void							ft_usleep(int time_to_usleep, t_philo *philo);

/* ACTIONS */

void							take_forks(t_philo *philo);
void							drop_the_forks(t_philo *philo);
bool							philo_cant_eat(t_philo *philo);
bool							philo_cant_sleep(t_philo *philo);
bool							philo_cant_think(t_philo *philo);

/* UTILS */

int								ft_atol(char *str);
long							get_time(void);
void							print(char *str, char *color, t_philo *philo);
bool							philo_is_dead(t_philo *philo);
bool							philo_ate_enough(t_philo *philo);

#endif