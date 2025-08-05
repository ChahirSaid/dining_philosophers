/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:07:48 by schahir           #+#    #+#             */
/*   Updated: 2025/08/05 11:50:01 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *monitoring(void *data)
{
    t_philo *philo;
    int i;

    philo = (t_philo *)data;
    if (check_n_delay(philo))
        return (NULL);
    usleep(200000);
    while (1)
    {
        i = 0;
        while (i < philo[i].schedule->nop)
        {
            pthread_mutex_lock(&philo[i].lock_mealtime);
            if (philo[i].schedule->ttd + philo[i].last_meal <= get_time())
            {
                pthread_mutex_unlock(&philo[i].lock_mealtime);
                pthread_mutex_lock(&philo[i].schedule->lock_death);
                philo[i].schedule->one_died = 1;
                pthread_mutex_unlock(&philo[i].schedule->lock_death);
                print_routine(philo, "philo die");
                exit(0);
                return (NULL);
            }
            pthread_mutex_unlock(&philo[i].lock_mealtime);
        }
        usleep(1000);
    }
    return (NULL);
}
