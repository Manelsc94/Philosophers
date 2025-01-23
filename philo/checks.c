/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:27:07 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/23 18:21:19 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_all_eaten(t_data *data, int i)
{
	if (i >= data->num_philo)
		return (true);
	if (!data->has_eaten[i])
		return (false);
	return (check_all_eaten(data, i + 1));
}

bool	check_philo_satisfaction(t_philo *philo)
{
	bool	all_eaten;

	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->meals_needed != -1 && philo->meals
		>= philo->data->meals_needed)
	{
		philo->ate_enough = true;
		philo->data->has_eaten[philo->id - 1] = true;
		all_eaten = check_all_eaten(philo->data, 0);
		if (all_eaten)
		{
			philo->data->stop_sim = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			return (true);
		}
	}
	pthread_mutex_unlock(&philo->data->stop_lock);
	return (false);
}

bool	check_philo_death(t_data *data, int i)
{
	long	current_time;
	long	time_since_last_meal;
	bool	is_dead;

	pthread_mutex_lock(&data->print_lock);
	current_time = get_timestamp();
	time_since_last_meal = current_time - data->philo[i].last_meal;
	if (time_since_last_meal > data->death_time)
	{
		pthread_mutex_lock(&data->stop_lock);
		if (!data->stop_sim)
		{
			data->stop_sim = 1;
			printf("\033[31m%ld %d died\033[0m\n",
				current_time - data->time_start, data->philo[i].id);
			is_dead = true;
		}
		pthread_mutex_unlock(&data->stop_lock);
		pthread_mutex_unlock(&data->print_lock);
		return (is_dead);
	}
	pthread_mutex_unlock(&data->print_lock);
	return (false);
}

void	check_philos(t_data *data, int i)
{
	if (i >= data->num_philo)
		return ;
	if (check_philo_death(data, i))
		return ;
	if (check_philo_satisfaction(&data->philo[i]))
		return ;
	check_philos(data, i + 1);
}

void	*check_pulse(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	while (true)
	{
		if (should_stop_sim(data))
			return (NULL);
		check_philos(data, 0);
		usleep(100);
	}
	return (NULL);
}
