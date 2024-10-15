/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:16 by gaperaud          #+#    #+#             */
/*   Updated: 2024/10/15 14:41:10 by gaperaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

// # define FORK "has taken a fork 🍴\n"
// # define EAT "is eating 🍝\n"
// # define SLEEP "is sleeping 🌙\n"
// # define THINK "is thinking 🤔\n"
// # define DEAD "%ld %d is dead 💀\n"
// # define MERROR "mutex initialisation error\n"

# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DEAD "%ld %d is dead\n"
# define MERROR "mutex initialisation error\n"

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define BLUE "\x1b[34m"
# define VIOLET "\x1b[35m"
# define GREY "\x1b[90m"
# define YELLOW "\x1b[33m"
# define RESET "\x1b[0m"

# define FSEM "/fork_semaphore"
# define PSEM "/print_semaphore"
# define DSEM "/death_semaphore"
# define WSEM "/waiter_semaphore"

typedef struct s_shared
{
	sem_t		*forks;
	sem_t		*print_sem;
	sem_t		*death_sem;
	sem_t		*waiter;
	int			forks_have_been_created;
	int			print_sem_have_been_created;
	int			death_sem_have_been_created;
	int			waiter_sem_have_been_created;
	pid_t		*pid_tab;
	int			pid_tab_has_been_created;
	int			simulation_must_stop;
	int			time_eaten;
}				t_shared;

typedef struct s_philo
{
	int			id;
	int			total_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_meal;
	long long	last_meal_time;
	int			start_time;
	int			last_meal;
	t_shared	*shared;
}				t_philo;

/* PARSING */

bool			arguments_are_not_valid(int ac, char **av);
t_shared		*create_shared_ressources(void);
bool			cant_init_semaphore(t_philo **philosophers);

/* EXIT */

int				wait_child(t_philo **philosopher);
void			clean_exit(t_philo **philos);

/* ACTIONS */

void			take_fork(t_philo *philo);
bool			philo_cant_eat(t_philo *philo);
bool			philo_cant_sleep(t_philo *philo);
bool			philo_cant_think(t_philo *philo);

/* UTILS */

int				ft_atoi(char *str);
long			get_time(void);
void			print(char *str, char *color, t_philo *philo);
bool			monitor(t_philo *philo);

#endif