/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:39:10 by schahir           #+#    #+#             */
/*   Updated: 2025/08/02 03:20:13 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_schedule
{
	int				nop;
	int				ttd;
	int				tte;
	int				tts;
	int				nom;
	int				departure;
	int				one_died;
	pthread_mutex_t	lock_print;
	pthread_mutex_t	lock_departure;
	pthread_mutex_t	lock_death;
}					t_schedule;

typedef struct s_philo
{
	pthread_t		philo;
	int				pid;
	pthread_mutex_t	lfork;
	pthread_mutex_t	*rfork;
	long			meals_eaten;
	long			first_meal;
	long			last_meal;
	t_schedule		*schedule;
}					t_philo;

long				ft_atoi(char *str);
void				*ft_calloc(size_t nmemb, size_t size);
void				putstr_fd(char *s, int fd);

#endif