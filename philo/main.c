/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:57:09 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/10 20:06:21 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_sim(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->stop_lock);
	free(data->has_eaten);
	free(data->philo);
}

void	start_sim(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philo[i].thread, NULL, philo_supper,
				&data->philo[i]) != 0)
		{
			write(2, "ERROR: Failed to create philosopher thread\n", 43);
			return ;
		}
		i++;
	}
	if (pthread_create(&data->philo->m_thread, NULL, check_pulse, data) != 0)
	{
		write(2, "ERROR: Failed to create monitor thread\n", 39);
		return ;
	}
	pthread_join(data->philo->m_thread, NULL);
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo[i].thread, NULL);
		i++;
	}
}

static int	parser(t_data *data, int argc, char **argv)
{
	if (!is_num(argv[1]) || !is_num(argv[2]) || !is_num(argv[3])
		|| !is_num(argv[4]) || (argc == 6 && !is_num(argv[5])))
		return (1);
	data->num_philo = ft_atoi(argv[1]);
	if (data->num_philo > 200)
		return (write(2, "That's way too many philosopher to feed!\n", 41), 1);
	data->death_time = ft_atoi(argv[2]);
	data->supper_time = ft_atoi(argv[3]);
	data->sleepy_time = ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_needed = ft_atoi(argv[5]);
	else
		data->meals_needed = -1;
	if (data->num_philo <= 0 || data->death_time <= 0 || data->supper_time <= 0
		|| data->sleepy_time <= 0 || (argc == 6 && data->meals_needed <= 0))
		return (1);
	return (0);
}

int	init_sim(t_data *data, int argc, char **argv)
{
    if (parser(data, argc, argv))
        return (write(2, "ERROR: Invalid args\n", 20), 1);
    data->time_start = get_timestamp();
    if (init_forks_and_philos(data) != 0)
        return (1);
    data->stop_sim = 0;
    data->philo_satisfied = 0;
    data->has_eaten = (bool *)malloc(data->num_philo * sizeof(bool));
    if (!data->has_eaten)
        return (write(2, "ERROR: Failed to allocate has_eaten array\n", 41), 1);
    int i = 0;
    while (i < data->num_philo)
    {
        data->has_eaten[i] = false;
        i++;
    }
    if (init_mutexes(data) != 0)
        return (1);
    return (0);
}

/* int	init_sim(t_data *data, int argc, char **argv)
{
	if (parser(data, argc, argv))
		return (write(2, "ERROR: Invalid args\n", 20), 1);
	data->time_start = get_timestamp();
	if (init_forks_and_philos(data) != 0)
		return (1);
	data->stop_sim = 0;
	data->philo_satisfied = 0;
	if (init_mutexes(data) != 0)
		return (1);
	return (0);
} */

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		write(2, "ERROR!\n", 7);
		printf("Hint: ./philo [num_philos] [time_to_die] [time_to_eat] ");
		printf("[time_to_sleep] (optional)[meals_required]\n");
		return (1);
	}
	if (init_sim(&data, argc, argv) != 0)
		return (1);
	start_sim(&data);
	clean_sim(&data);
	return (0);
}
