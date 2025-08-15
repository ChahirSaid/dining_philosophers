/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:07:48 by schahir           #+#    #+#             */
/*   Updated: 2025/08/15 15:54:23 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_full(t_philo *philo)
{
	int	i;
	int	n;

	n = philo->schedule->nop;
	i = 0;
	while (i < n)
	{
		pthread_mutex_lock(&philo[i].lock_state);
		if (philo[i].state == 0)
			return (pthread_mutex_unlock(&philo[i].lock_state), 1);
		pthread_mutex_unlock(&philo[i].lock_state);
		i++;
	}
	return (0);
}

static void	print_death(t_philo *philo, int i)
{
	pthread_mutex_unlock(&philo[i].lock_mealtime);
	pthread_mutex_lock(&philo[i].schedule->lock_death);
	philo[i].schedule->one_died = 1;
	pthread_mutex_unlock(&philo[i].schedule->lock_death);
	pthread_mutex_lock(&philo->schedule->lock_print);
	printf("%ld\t%d %s\n", get_time() - philo[i].schedule->first_meal,
		philo[i].pid, "died");
	pthread_mutex_unlock(&philo->schedule->lock_print);
}

static int	check_deaths(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo[i].lock_mealtime);
	pthread_mutex_lock(&philo[i].lock_state);
	if (!philo[i].state && philo[i].schedule->ttd
		+ philo[i].last_meal <= get_time())
	{
		print_death(philo, i);
		pthread_mutex_unlock(&philo[i].lock_state);
		return (1);
	}
	pthread_mutex_unlock(&philo[i].lock_state);
	pthread_mutex_unlock(&philo[i].lock_mealtime);
	return (0);
}

void	*monitoring(void *data)
{
	t_philo	*philo;
	int		i;
	int		num;

	philo = (t_philo *)data;
	if (check_n_delay(philo))
		return (NULL);
	num = philo[0].schedule->nop;
	while (1)
	{
		i = 0;
		if (!check_full(philo))
			return (NULL);
		while (i < num)
		{
			if (check_deaths(philo, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
