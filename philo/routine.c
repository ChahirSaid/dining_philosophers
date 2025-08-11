/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 09:38:06 by schahir           #+#    #+#             */
/*   Updated: 2025/08/11 16:40:29 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_n_delay(t_philo *philo)
{
	pthread_mutex_lock(&philo->schedule->lock_departure);
	if (!philo->schedule->departure)
	{
		pthread_mutex_unlock(&philo->schedule->lock_departure);
		return (-1);
	}
	pthread_mutex_unlock(&philo->schedule->lock_departure);
	if (philo->pid % 2 && philo->schedule->nop > 1)
	{
		usleep(1000);
		print_routine(philo, "is thinking");
		if (philo->pid == 1 && philo->schedule->nop % 2)
			usleep((philo->schedule->tte + 5) * 1000);
	}
	return (0);
}

static void	lock_forks(t_philo *philo, int *n)
{
	if (!(philo->pid == philo->schedule->nop))
	{
		pthread_mutex_lock(&philo->lfork);
		if (print_routine(philo, "has taken a fork"))
			*n = 1;
		pthread_mutex_lock(philo->rfork);
		if (!*n && print_routine(philo, "has taken a fork"))
			*n = 1;
	}
	else
	{
		pthread_mutex_lock(philo->rfork);
		if (!*n && print_routine(philo, "has taken a fork"))
			*n = 1;
		if (!*n && philo->schedule->nop == 1)
		{
			*n = 1;
			philo->mutexed = 1;
			return ;
		}
		pthread_mutex_lock(&philo->lfork);
		if (print_routine(philo, "has taken a fork"))
			*n = 1;
	}
}

static int	eating(t_philo *philo)
{
	int	n;

	n = 0;
	lock_forks(philo, &n);
	if (!n && print_routine(philo, "is eating"))
		n = 1;
	pthread_mutex_lock(&philo->lock_mealtime);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->lock_mealtime);
	if (!n && usleep(philo->schedule->tte * 1000))
		n = 1;
	if (philo->schedule->nop != 1)
		pthread_mutex_unlock(&philo->lfork);
	pthread_mutex_unlock(philo->rfork);
	pthread_mutex_lock(&philo->lock_state);
	philo->meals_eaten++;
	if (philo->meals_eaten == philo->schedule->nom)
	{
		if (philo->schedule->nop != 1)
			philo->state = 1;
		return (pthread_mutex_unlock(&philo->lock_state), 1);
	}
	pthread_mutex_unlock(&philo->lock_state);
	return (n);
}

static int	thinking(t_philo *philo)
{
	int	to_think;

	to_think = 1;
	if (print_routine(philo, "is thinking"))
		return (-1);
	if (philo->schedule->nop % 2)
	{
		to_think = philo->schedule->tte;
		if (philo->schedule->tte < philo->schedule->tts)
		{
			to_think -= philo->schedule->tts - philo->schedule->tte;
			if (to_think < 0)
				to_think = 0;
		}
		else if (philo->schedule->tte > philo->schedule->tts)
			to_think += philo->schedule->tte - philo->schedule->tts;
	}
	else
		to_think = 1;
	usleep(to_think * 1000);
	return (0);
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
		if (thinking(philo))
			return (NULL);
	}
	return (NULL);
}
