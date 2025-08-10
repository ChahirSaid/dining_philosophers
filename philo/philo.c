/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:13:07 by schahir           #+#    #+#             */
/*   Updated: 2025/08/10 22:24:36 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_schedule *s)
{
	if (pthread_mutex_init(&s->lock_print, NULL))
		return (-1);
	if (pthread_mutex_init(&s->lock_departure, NULL))
		return (pthread_mutex_destroy(&s->lock_print), -1);
	if (pthread_mutex_init(&s->lock_death, NULL))
		return (pthread_mutex_destroy(&s->lock_print),
			pthread_mutex_destroy(&s->lock_departure), -1);
	return (0);
}

static int	populate_philos(t_schedule *s, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < s->nop)
	{
		philo[i].pid = i + 1;
		philo[i].schedule = s;
		if (pthread_mutex_init(&philo[i].lfork, NULL))
			return (-1);
		if (i + 1 == s->nop)
			philo[i].rfork = &philo[0].lfork;
		else
			philo[i].rfork = &philo[i + 1].lfork;
		i++;
	}
	return (0);
}

static int	launch_simulation(t_schedule *s, t_philo *philo)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	pthread_mutex_lock(&s->lock_departure);
	while (i < s->nop)
	{
		philo[i].last_meal = get_time();
		if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
		{
			pthread_mutex_unlock(&s->lock_departure);
			while (--i > 0)
				pthread_join(philo[i].philo, NULL);
			return (-1);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, monitoring, philo))
	{
		pthread_mutex_unlock(&s->lock_departure);
		while (--i > 0)
			pthread_join(philo[i].philo, NULL);
		return (-1);
	}
	s->departure = 1;
	pthread_mutex_unlock(&s->lock_departure);
	pthread_join(monitor, NULL);
	while (i--)
		pthread_join(philo[i].philo, NULL);
	return (0);
}

static int	check_n_set(t_schedule *s)
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

int	main(int ac, char **av)
{
	t_philo		*philo;
	t_schedule	s;

	if (ac != 5 && ac != 6)
		return (putstr_fd("error: invalid arguments\n", 2), 1);
	if (ac == 6)
		s = (t_schedule){.nop = ft_atoi(av[1]), .ttd = ft_atoi(av[2]),
			.tte = ft_atoi(av[3]), .tts = ft_atoi(av[4]), .nom = ft_atoi(av[5]),
			.departure = 0, .one_died = 0};
	else
		s = (t_schedule){.nop = ft_atoi(av[1]), .ttd = ft_atoi(av[2]),
			.tte = ft_atoi(av[3]), .tts = ft_atoi(av[4]), .nom = -2,
			.departure = 0, .one_died = 0};
	if (check_n_set(&s))
		return (putstr_fd("error: invalid arguments\n", 2), 1);
	if (init_mutexes(&s))
		return (putstr_fd("error: failed to initialize mutexes\n", 2), 1);
	philo = ft_calloc(s.nop, sizeof(t_philo));
	if (!philo)
		return (print_n_destroy(&s, "error: allocation failed\n"), 1);
	if (populate_philos(&s, philo))
		return (clean_print(philo, "error: failed to initialize mutexes\n"), 1);
	if (launch_simulation(&s, philo))
		return (clean_print(philo, "error: failed to create threads\n"), 1);
	return (clean_print(philo, NULL), 0);
}
