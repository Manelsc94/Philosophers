/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:31:44 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/14 15:15:57 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
	str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	is_num(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_action(t_data *data, int id, const char *action)
{
	const char	*color;

	pthread_mutex_lock(&data->stop_lock);
	if (data->stop_sim && ft_strcmp(action, "is sleeping") == 0)
	{
		pthread_mutex_unlock(&data->stop_lock);
		return ;
	}
	pthread_mutex_unlock(&data->stop_lock);
	if (ft_strcmp(action, "is thinking") == 0)
		color = "\033[34m";
	else if (ft_strcmp(action, "is eating") == 0)
		color = "\033[32m";
	else if (ft_strcmp(action, "is sleeping") == 0)
		color = "\033[33m";
	else if (ft_strcmp(action, "has taken a fork") == 0)
		color = "\033[36m";
	else
		color = "\033[0m";
	pthread_mutex_lock(&data->print_lock);
	printf("%s%ld %d %s\033[0m\n", color, get_timestamp() - data->time_start,
		id, action);
	pthread_mutex_unlock(&data->print_lock);
}
