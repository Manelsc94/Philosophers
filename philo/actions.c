/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:31:20 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/14 15:39:19 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	thinking(t_philo *philo)
{
	log_action(philo->data, philo->id, "is thinking");
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->ate_enough || philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (true);
}

bool	sleeping(t_philo *philo)
{
	log_action(philo->data, philo->id, "is sleeping");
	if (!sleep_for(philo->data->sleepy_time, philo->data))
		return (false);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->ate_enough || philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (true);
}

bool	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_lock);
	philo->last_meal = get_timestamp();
	pthread_mutex_unlock(&philo->data->print_lock);
	log_action(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->stop_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->data->stop_lock);
	if (!sleep_for(philo->data->supper_time, philo->data))
		return (release_forks(philo), false);
	release_forks(philo);
	return (true);
}

bool	lock_forks(t_philo *philo, pthread_mutex_t *ff, pthread_mutex_t *sf)
{
	pthread_mutex_lock(ff);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		pthread_mutex_unlock(ff);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	log_action(philo->data, philo->id, "has taken a fork");
	pthread_mutex_lock(sf);
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		pthread_mutex_unlock(ff);
		pthread_mutex_unlock(sf);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	log_action(philo->data, philo->id, "has taken a fork");
	return (true);
}

bool	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->stop_sim)
	{
		pthread_mutex_unlock(&philo->data->stop_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (lock_forks(philo, first_fork, second_fork));
}
