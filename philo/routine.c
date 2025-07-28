/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:39:48 by moel-yag          #+#    #+#             */
/*   Updated: 2025/07/28 14:04:27 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_usleep(philo->time_to_eat, philo);
}

static int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(philo->stop_mutex);
	stop = *(philo->stop);
	pthread_mutex_unlock(philo->stop_mutex);
	return (stop);
}

static void	routine_loop_body(t_philo *philo)
{
	take_forks(philo);
	eat(philo);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	print_status(philo, "is sleeping");
	precise_usleep(philo->time_to_sleep, philo);
	print_status(philo, "is thinking");
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->left_fork == philo->right_fork)
	{
		print_status(philo, "has taken a fork");
		precise_usleep(philo->time_to_die, philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (should_stop(philo))
			break ;
		routine_loop_body(philo);
		usleep(500); // Small sleep to prevent busy waiting
	}
	return (NULL);
}
