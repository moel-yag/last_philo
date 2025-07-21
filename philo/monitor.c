/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:08:44 by moel-yag          #+#    #+#             */
/*   Updated: 2025/07/09 17:34:08 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_sim *sim, int i)
{
	pthread_mutex_lock(&sim->philos[i].meal_mutex);
	if (get_time() - sim->philos[i].last_meal > sim->time_to_die)
	{
		pthread_mutex_lock(sim->stop_mutex);
		if (!*(sim->stop))
		{
			*(sim->stop) = 1;
			pthread_mutex_lock(sim->print_mutex);
			printf("%lld %d died\n", get_time() - sim->philos[i].start_time,
				sim->philos[i].id);
			pthread_mutex_unlock(sim->print_mutex);
		}
		pthread_mutex_unlock(sim->stop_mutex);
		pthread_mutex_unlock(&sim->philos[i].meal_mutex);
	}
	else
		pthread_mutex_unlock(&sim->philos[i].meal_mutex);
}

static int	all_philos_full(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philo)
	{
		pthread_mutex_lock(&sim->philos[i].meal_mutex);
		if (sim->philos[i].meals_eaten < sim->meal_target)
		{
			pthread_mutex_unlock(&sim->philos[i].meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sim->philos[i].meal_mutex);
		i++;
	}
	return (1);
}

static int	should_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(sim->stop_mutex);
	stop = *(sim->stop);
	pthread_mutex_unlock(sim->stop_mutex);
	return (stop);
}

void	*monitor(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->num_philo)
		{
			check_death(sim, i);
			if (should_stop(sim))
				return (NULL);
			i++;
		}
		if (sim->meal_target > 0 && all_philos_full(sim))
		{
			pthread_mutex_lock(sim->stop_mutex);
			*(sim->stop) = 1;
			pthread_mutex_unlock(sim->stop_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}
