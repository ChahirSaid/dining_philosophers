/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:07:48 by schahir           #+#    #+#             */
/*   Updated: 2025/08/09 21:46:48 by schahir          ###   ########.fr       */
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
    int num = philo[0].schedule->nop;
    while (1)
    {
        i = 0;
        while (i < num)
        {
            pthread_mutex_lock(&philo[i].lock_mealtime);
            if (philo[i].schedule->ttd + philo[i].last_meal <= get_time())
            {
                pthread_mutex_unlock(&philo[i].lock_mealtime);
                pthread_mutex_lock(&philo->schedule->lock_meal_limit);
                if (philo->schedule->meals_limit == (philo->schedule->nop * philo->schedule->nom))
                    return (pthread_mutex_unlock(&philo->schedule->lock_meal_limit), NULL);
                pthread_mutex_unlock(&philo->schedule->lock_meal_limit);
                pthread_mutex_lock(&philo[i].schedule->lock_death);
                philo[i].schedule->one_died = 1;
                pthread_mutex_unlock(&philo->schedule->lock_death);
	            pthread_mutex_lock(&philo->schedule->lock_print);
	            printf("%ld\t%d %s\n", get_time() - philo->schedule->first_meal,
			    philo->pid,"died");
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
