/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:31:20 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/10 20:03:15 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	sleep_for(long ms, t_data *data)
{
	long	start;

	start = get_timestamp();
	while (get_timestamp() - start < ms)
	{
		usleep(100);
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

bool	sleep_and_think(t_philo *philo)
{
	log_action(philo->data, philo->id, "is sleeping");
	if (!sleep_for(philo->data->sleepy_time, philo->data))
		return (false);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->ate_enough)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	log_action(philo->data, philo->id, "is thinking");
	usleep(100);
	return (true);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

// Modify the eat function in actions.c to mark when a philosopher eats:
bool    eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->print_lock);
    philo->last_meal = get_timestamp();
    pthread_mutex_unlock(&philo->data->print_lock);
    log_action(philo->data, philo->id, "is eating");
    
    // Mark that this philosopher has eaten
    pthread_mutex_lock(&philo->data->stop_lock);
    philo->data->has_eaten[philo->id - 1] = true;
    pthread_mutex_unlock(&philo->data->stop_lock);
    
    if (!sleep_for(philo->data->supper_time, philo->data))
        return (release_forks(philo), false);
    pthread_mutex_lock(&philo->data->stop_lock);
    philo->meals++;
    pthread_mutex_unlock(&philo->data->stop_lock);
    release_forks(philo);
    return (true);
}

/* bool	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	philo->last_meal = get_timestamp();
	pthread_mutex_unlock(&philo->data->print_lock);
	log_action(philo->data, philo->id, "is eating");
	if (!sleep_for(philo->data->supper_time, philo->data))
		return (release_forks(philo), false);
	pthread_mutex_lock(&philo->data->stop_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->data->stop_lock);
	release_forks(philo);
	return (true);
} */

bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_sim
		|| (philo->data->meals_needed != -1
			&& philo->data->philo_satisfied >= philo->data->num_philo))
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		log_action(philo->data, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		log_action(philo->data, philo->id, "has taken a fork");
	}
	return (true);
}
