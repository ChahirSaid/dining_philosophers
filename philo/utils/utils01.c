/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils01.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:04:23 by schahir           #+#    #+#             */
/*   Updated: 2025/08/15 15:55:20 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	print_routine(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->schedule->lock_print);
	pthread_mutex_lock(&philo->schedule->lock_death);
	if (philo->schedule->one_died)
	{
		pthread_mutex_unlock(&philo->schedule->lock_death);
		pthread_mutex_unlock(&philo->schedule->lock_print);
		return (1);
	}
	pthread_mutex_unlock(&philo->schedule->lock_death);
	printf("%ld\t%d %s\n", get_time() - philo->schedule->first_meal, philo->pid,
		msg);
	pthread_mutex_unlock(&philo->schedule->lock_print);
	return (0);
}

int	check_n_set(t_schedule *s)
{
	if (s->nop <= 0 || s->ttd <= 0 || s->tte <= 0 || s->tts <= 0 || s->nom == -1
		|| s->nom == 0)
		return (1);
	if (s->tte > s->ttd)
		s->tte = s->ttd;
	if (s->tts > s->ttd)
		s->tts = s->ttd;
	s->first_meal = get_time();
	return (0);
}

void	print_n_destroy(t_schedule *s, char *msg)
{
	pthread_mutex_destroy(&s->lock_print);
	pthread_mutex_destroy(&s->lock_departure);
	pthread_mutex_destroy(&s->lock_death);
	if (msg)
		putstr_fd(msg, 2);
}

void	clean_print(t_philo *philo, char *msg)
{
	pthread_mutex_destroy(&philo->lfork);
	print_n_destroy(philo->schedule, msg);
	free(philo);
}
