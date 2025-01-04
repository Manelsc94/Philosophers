/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:27:07 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/03 19:10:47 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_philo_satisfaction(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_lock);
	if (philo->data->meals_needed != -1 && philo->meals
		>= philo->data->meals_needed)
	{
		philo->ate_enough = true;
		philo->data->philo_satisfied++;
		if (philo->data->philo_satisfied >= philo->data->num_philo)
		{
			philo->data->stop_sim = 1;
			pthread_mutex_unlock(&philo->data->stop_lock);
			//release_forks(philo);
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

	pthread_mutex_lock(&data->print_lock);
	current_time = get_timestamp();
	time_since_last_meal = current_time - data->philo[i].last_meal;
	if (time_since_last_meal > data->death_time)
	{
		pthread_mutex_lock(&data->stop_lock);
		data->stop_sim = 1;
		pthread_mutex_unlock(&data->stop_lock);
		printf("\033[31m%ld %d died\033[0m\n",
			current_time - data->time_start, data->philo[i].id);
		pthread_mutex_unlock(&data->print_lock);
		return (true);
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
		usleep(1000);
	}
	return (NULL);
}
