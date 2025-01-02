/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaperaud <gaperaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:00:16 by gaperaud          #+#    #+#             */
/*   Updated: 2025/01/02 04:47:05 by gaperaud         ###   ########.fr       */
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
# define DEAD "died\n"
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

# define FSEM "/fork_semaphore"
# define PSEM "/print_semaphore"
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
	sem_t		*forks;
	sem_t		*print;
	sem_t		*waiter;
	pthread_t	monitor;
	pid_t		*pid_tab;
	int			exit_value;
}				t_philo;

/* UTILS */

void			print(char *str, char *color, t_philo *philo);
void			ft_usleep(long time, t_philo *philo);
void			check_input(int ac, char **av);
int				ft_atol(char *str);
long			get_time(void);

#endif