/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:13:07 by schahir           #+#    #+#             */
/*   Updated: 2025/07/31 18:22:14 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *routine(void *data)
{
    t_philo *infos;
    infos = (t_philo *)data;
    printf("%d\n", infos->pid);
    return (NULL);
}

int main(int ac, char **av)
{
    int         i;
    t_philo     *philo;
    t_schedule  s;

    if (ac != 5 && ac != 6)
        return(putstr_fd("error: invalid arguments\n", 2), 1);
    if (ac == 6)
        s = (t_schedule){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]), ft_atoi(av[5])};
    else
        s = (t_schedule){ft_atoi(av[1]), ft_atoi(av[2]), ft_atoi(av[3]), ft_atoi(av[4]), -2};
    
    if (s.nop <= 0 || s.ttd == -1 || s.tte == -1 || s.tts == -1 || s.nom == -1)
        return(putstr_fd("error: invalid arguments\n", 2), 1);    
    philo = calloc(s.nop, sizeof(t_philo));
    if (!philo)
        return (1);
    //assign pid
    i = 0;
    while (i < s.nop)
    {
        philo[i].pid = i + 1;
        i++;
    }
    i = 0;
    while (i < s.nop)
    {
        pthread_create(&philo[i].philo, NULL, routine, &philo[i]);
        i++;
    }
    i = 0;
    while (i < s.nop)
    {
        pthread_join(philo[i].philo, NULL);
        i++;
    }
    free(philo);
    return (0);
}
