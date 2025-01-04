/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:57:02 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/03 19:04:39 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define _GNU_SOURCE

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

// ENUMS
typedef enum e_status
{
	FAILURE,
	SUCCESS
}	t_status;

// STRUCTS
typedef struct s_philo
{
	int				id;
	int			meals;

	long			last_meal;

	pthread_t		thread;
	pthread_t		m_thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	struct s_data	*data;

	bool			ate_enough;
	bool			meals_had;
}	t_philo;

typedef struct s_data
{
	int				num_philo;
	int				death_time;
	int				supper_time;
	int				sleepy_time;
	int				meals_needed;
	int				stop_sim;
	int				philo_satisfied;

	long			time_start;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	meals_lock;

	t_philo			*philo;
}	t_data;

//main.c
void	clean_sim(t_data *data);
void	start_sim(t_data *data);
int		init_sim(t_data *data, int argc, char **argv);

//init.c
int		init_forks(t_data *data);
int		init_philos(t_data *data);
int		init_forks_and_philos(t_data *data);
int		init_mutexes(t_data *data);

//table.c
bool	should_stop_sim(t_data *data);
bool	should_stop(t_philo *philo);
void	*philo_supper(void *args);

//actions.c
bool	sleep_for(long ms, t_data *data);
bool	sleep_and_think(t_philo *philo);
void	release_forks(t_philo *philo);
bool	eat(t_philo *philo);
bool	take_forks(t_philo *philo);

//checks.c
bool	check_philo_death(t_data *data, int i);
void	check_philos(t_data *data, int i);
void	*check_pulse(void *args);

//utilities.c
int		ft_atoi(const char *str);
int		is_num(const char *str);
int		ft_strcmp(const char *s1, const char *s2);
long	get_timestamp(void);
void	log_action(t_data *data, int id, const char *action);

#endif