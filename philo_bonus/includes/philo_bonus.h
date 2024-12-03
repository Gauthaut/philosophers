/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legoat <legoat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:16 by gaperaud          #+#    #+#             */
/*   Updated: 2024/12/03 04:45:42 by legoat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
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
# define DEAD "died\n"
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
# define SSEM "/stop_simulation_semaphore"
# define WSEM "/waiter_simulation"
# define MSEM "/meal_counter_sem"

typedef struct s_philo
{
	int			id;
	int			total_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			time_to_think;
	int			number_of_meal;
	long		start_time;
	long		last_meal_time;
	int			time_eaten;
	int			child_must_stop;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*stop_simulation_sem;
	sem_t		*waiter;
	sem_t		**child_monitor;
	sem_t		*meal_counter;
	pthread_t	monitor_thread;
	pthread_t	meal_monitor;
	pid_t		*pid_tab;
}				t_philo;

/* PARSING */

bool			arguments_are_not_valid(int ac, char **av);
void			init_philo(t_philo *philo, char **av);
char			*get_sem_name(char type, int index);
bool			cant_init_semaphore(t_philo *philo);

/* MONITEUR */

void			*meal_monitor(void *args);
bool			cant_run_meal_monitor(t_philo *philo);
void			stop_simulation(t_philo *philo, pid_t *pid_tab);
void			close_sem(t_philo *philo, int last_index);
void			unlink_sem(int last_index);

/* ACTIONS */

bool			philo_cant_eat(t_philo *philo);
bool			philo_cant_sleep(t_philo *philo);
bool			philo_cant_think(t_philo *philo);

/* UTILS */

int				ft_strlen(char *str);
int				ft_atoi(char *str);
long			get_time(void);
void			print(char *str, char *color, t_philo *philo);

#endif