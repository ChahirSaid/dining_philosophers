/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:39:10 by schahir           #+#    #+#             */
/*   Updated: 2025/08/10 15:02:16 by schahir          ###   ########.fr       */
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
	long			first_meal;
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
	pthread_mutex_t	lock_mealtime;
	long			meals_eaten;
	pthread_mutex_t	lock_state;
	int				state;
	long			last_meal;
	t_schedule		*schedule;
}					t_philo;

/*-----------Philo-------------*/
void	*routine(void *data);
int	print_routine(t_philo *philo, char *msg);
int  check_n_delay(t_philo *philo);
/*-------Utils----------*/
int				ft_atoi(char *str);
void				*ft_calloc(size_t nmemb, size_t size);
void				putstr_fd(char *s, int fd);
void				print_n_destroy(t_schedule *s, char *msg);
void				print_n_clean(t_philo *philo, char *msg);
long				get_time();

void    *monitoring(void *data);
#endif