/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:13:07 by schahir           #+#    #+#             */
/*   Updated: 2025/08/02 01:39:17 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int    init_mutexes(t_schedule *s)
{
    if (pthread_mutex_init(&s->lock_print, NULL))
        return (-1);
    if (pthread_mutex_init(&s->lock_departure, NULL))
        return (pthread_mutex_destroy(&s->lock_print), -1);
    if (pthread_mutex_init(&s->lock_death, NULL))
        return(pthread_mutex_destroy(&s->lock_print),
                pthread_mutex_destroy(&s->lock_departure),-1);
    return (0);
}

int    populate_philos(t_schedule *s, t_philo *philo)
{
    int i;

    i = 0;
    while (i < s->nop)
    {
        philo[i].pid = i + 1;
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

void    *routine(void *data)
{
    t_philo *philo;
    philo = (t_philo *)data;
    // while (1)
    // {
    //     picking_forks();
    //     eating();
    //     sleeping();
    //     thinking();
        
    // }
    return (NULL);
}

int    launch_simulation(t_schedule *s, t_philo *philo)
{
    int i;

    i = 0;
    pthread_mutex_lock(&s->lock_departure);
    while (i < s->nop)
    {
        if (pthread_create(&philo[i].philo, NULL, routine, &philo[i]))
        {
            pthread_mutex_unlock(&s->lock_departure);
            while (--i > 0)
                pthread_join(philo[i].philo, NULL);
            return (-1);
        }
        i++;
    }
    s->departure = 1;
    pthread_mutex_unlock(&s->lock_departure);
    while (i--)
            pthread_join(philo[i].philo, NULL);
    return (0);
}

int main(int ac, char **av)
{
    t_philo     *philo;
    t_schedule  s;
    int         i;

    if (ac != 5 && ac != 6)
        return(putstr_fd("error: invalid arguments\n", 2), 1);
    if (ac == 6)
        s = (t_schedule){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]), ft_atoi(av[5]), 0, 0};
    else
        s = (t_schedule){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]), -2, 0, 0};
    if (s.nop <= 0 || s.ttd == -1 || s.tte == -1 || s.tts == -1 || s.nom == -1)
        return(putstr_fd("error: invalid arguments\n", 2), 1);
    if (init_mutexes(&s))
        return (putstr_fd("error: failed to initialize mutexes\n", 2), 1);
    philo = ft_calloc(s.nop, sizeof(t_philo));
    if (!philo)
        return (putstr_fd("error: allocation failed\n", 2), 1);
    if (populate_philos(&s, philo))
        return (free(philo), putstr_fd("error: failed to initialize mutexes\n", 2), 1);
    if (launch_simulation(&s, philo))
        return (free(philo), putstr_fd("error: failed to create threads\n", 2), 1);
    free(philo);
    return (0);
}
