/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-yag <moel-yag@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:08:06 by moel-yag          #+#    #+#             */
/*   Updated: 2025/07/13 18:54:18 by moel-yag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_cleanup_data(t_cleanup_data *c, t_main_data *d)
{
	c->philos = d->philos;
	c->forks = d->forks;
	c->stop_mutex = &d->stop_mutex;
	c->print_mutex = &d->print_mutex;
	c->num_philo = d->num_philo;
}

void	cleanup(t_cleanup_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
	}
	pthread_mutex_destroy(data->stop_mutex);
	pthread_mutex_destroy(data->print_mutex);
}
