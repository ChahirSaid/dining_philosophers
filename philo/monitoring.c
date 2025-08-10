/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:07:48 by schahir           #+#    #+#             */
/*   Updated: 2025/08/10 16:12:34 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_all_full(t_philo *philo)
{
    int i;
    int n;

    n = philo->schedule->nop;
    i = 0;
    while (i < n)
    {
        pthread_mutex_lock(&philo[i].lock_state);
        if (philo[i].state == 0)
            return (pthread_mutex_unlock(&philo[i].lock_state),1);
        pthread_mutex_unlock(&philo[i].lock_state);
        i++;
    }
    return (0);
}

void    *monitoring(void *data)
{
    t_philo *philo;
    int i;
    
    philo = (t_philo *)data;
    if (check_n_delay(philo))
        return (NULL);
    int num = philo[0].schedule->nop;
    while (1)
    {
        i = 0;
        if (!check_all_full(philo))
            return (NULL);
        while (i < num)
        {
            pthread_mutex_lock(&philo[i].lock_mealtime);
            if (!philo[i].state && philo[i].schedule->ttd + philo[i].last_meal <= get_time())
            {
                pthread_mutex_unlock(&philo[i].lock_mealtime);
                pthread_mutex_lock(&philo[i].schedule->lock_death);
                philo[i].schedule->one_died = 1;
                pthread_mutex_unlock(&philo->schedule->lock_death);
	            pthread_mutex_lock(&philo->schedule->lock_print);
	            printf("%ld\t%d %s\n", get_time() - philo[i].schedule->first_meal,
			    philo[i].pid,"died");
	            pthread_mutex_unlock(&philo->schedule->lock_print);
                return (NULL);
            }
            pthread_mutex_unlock(&philo[i].lock_mealtime);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}
