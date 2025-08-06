/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:38:06 by schahir           #+#    #+#             */
/*   Updated: 2025/08/06 11:43:02 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int  check_n_delay(t_philo *philo)
{
    pthread_mutex_lock(&philo->schedule->lock_departure);
	if (!philo->schedule->departure)
	{
        pthread_mutex_unlock(&philo->schedule->lock_departure);
		return (-1);
	}
	pthread_mutex_unlock(&philo->schedule->lock_departure);
    if (philo->pid % 2)
  	  usleep(1000);
    return (0);
}


static int  eating(t_philo *philo)
{
	int n = 0;
	pthread_mutex_lock(&philo->lfork);
	if (print_routine(philo, "has taken a fork"))
		n = 1;
	pthread_mutex_lock(philo->rfork);
	if (!n && print_routine(philo, "has taken a fork"))
		n = 1;
	if (!n && print_routine(philo, "is eating"))
		n = 1;
	pthread_mutex_lock(&philo->lock_mealtime);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->lock_mealtime);
	if (!n && usleep(philo->schedule->tte * 1000))
		n = 1;
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_unlock(&philo->lfork);
	if (philo->meals_eaten == philo->schedule->nom)
		return (1);
	return (n);
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (check_n_delay(philo))
        return (NULL);
	while (1)
	{
	    if (eating(philo))
			return (NULL);
		if (print_routine(philo, "is sleeping"))
			return (NULL);
		usleep(philo->schedule->tts * 1000);
		if (print_routine(philo, "is thinking"))
			return (NULL);
		usleep (1000);
	}
	return (NULL);
}
