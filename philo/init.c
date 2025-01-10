/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:52:16 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/04 17:42:36 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	int	i;

	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philo);
	if (!data->forks)
		return (write(2, "Error allocating memory for forks.\n", 35), 1);
	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (write(2, "ERROR: Failed to init fork mutex\n", 33), 1);
		i++;
	}
	return (0);
}

int	allocate_philos(t_data *data)
{
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->num_philo);
	if (!data->philo)
		return (write(2, "Error allocating memory for philosophers.\n", 42), 1);
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	if (allocate_philos(data) != 0)
		return (1);
	i = 0;
	while (i < data->num_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].last_meal = data->time_start;
		data->philo[i].meals = 0;
		if (i == 0)
		{
			data->philo[i].left_fork = &data->forks[data->num_philo - 1];
			data->philo[i].right_fork = &data->forks[i];
		}
		else
		{
			data->philo[i].left_fork = &data->forks[i - 1];
			data->philo[i].right_fork = &data->forks[i];
		}
		data->philo[i].data = data;
		data->philo[i].ate_enough = false;
		//data->philo_satisfied = 0;
		i++;
	}
	return (0);
}

int	init_forks_and_philos(t_data *data)
{
	if (init_forks(data) != 0)
		return (1);
	if (init_philos(data) != 0)
		return (1);
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (write(2, "ERROR: Failed to init print mutex\n", 34), 1);
	if (pthread_mutex_init(&data->stop_lock, NULL) != 0)
	{
		write(2, "ERROR: Failed to init stop mutex\n", 34);
		return (1);
	}
	return (0);
}
