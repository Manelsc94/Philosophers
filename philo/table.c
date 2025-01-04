/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:11:48 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/03 18:30:56 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	*philo_supper(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (true)
	{
		if (philo->data->num_philo == 1)
		{
			log_action(philo->data, philo->id, "has taken a fork");
			sleep_for(philo->data->death_time, philo->data);
			return (NULL);
		}
		if (should_stop(philo))
			return (NULL);
		if (!take_forks(philo))
			return (NULL);
		if (!eat(philo))
			return (NULL);
		if (!sleep_and_think(philo))
			return (NULL);
	}
	return (NULL);
}
