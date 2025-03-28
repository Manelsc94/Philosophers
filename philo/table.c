/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:11:48 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/23 19:30:17 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
} */

bool	sleep_for(long ms, t_data *data)
{
	long	start;

	start = get_timestamp();
	while (get_timestamp() - start < ms)
	{
		pthread_mutex_lock(&data->stop_lock);
		if (data->stop_sim)
		{
			pthread_mutex_unlock(&data->stop_lock);
			return (false);
		}
		pthread_mutex_unlock(&data->stop_lock);
	}
	return (true);
}

bool	should_stop_sim(t_data *data)
{
	bool	stop_sim;

	pthread_mutex_lock(&data->stop_lock);
	stop_sim = data->stop_sim;
	pthread_mutex_unlock(&data->stop_lock);
	return (stop_sim);
}

bool	should_stop(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (false);
}

void	*supper_loop(t_philo *philo)
{
	long	think_time;

	while (true)
	{
		if (should_stop(philo))
			return (NULL);
		think_time = (philo->data->death_time - philo->data->supper_time
				- philo->data->sleepy_time) / 2;
		if (think_time > 0)
			sleep_for(think_time, philo->data);
		if (!take_forks(philo))
			return (NULL);
		if (!eating(philo))
			return (NULL);
		if (!sleeping(philo))
			return (NULL);
		if (!thinking(philo))
			return (NULL);
	}
	return (NULL);
}

void	*philo_supper(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->id % 2 == 0)
		usleep((philo->data->supper_time / 2) * 1000);
	if (philo->data->num_philo == 1)
	{
		log_action(philo->data, philo->id, "has taken a fork");
		sleep_for(philo->data->death_time, philo->data);
		return (NULL);
	}
	return (supper_loop(philo));
}
