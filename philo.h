/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:39:10 by schahir           #+#    #+#             */
/*   Updated: 2025/07/31 18:10:29 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_schedule{
    int nop;
    int ttd;
    int tte;
    int tts;
    int nom;
}   t_schedule;

typedef struct s_philo
{
    pthread_t   philo;
    int         pid;
    t_schedule  *schedule;
}   t_philo;

long    ft_atoi(char *str);
void	putstr_fd(char *s, int fd);

#endif