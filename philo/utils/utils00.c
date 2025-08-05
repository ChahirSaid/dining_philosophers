/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schahir <schahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:39:07 by schahir           #+#    #+#             */
/*   Updated: 2025/08/05 10:55:25 by schahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	ft_atoi(char *str)
{
	long (value), (i);
	i = 0;
	value = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+')
		i++;
	if (str[i] == '-' && str[i + 1] && str[i + 1] == '0')
	{
		while (str[++i] == '0')
			;
		if (str[i])
			return (-1);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		value = value * 10 + (str[i] - '0');
		i++;
		if (value > LONG_MAX)
			return (-1);
	}
	if (str[i])
		return (-1);
	return (value);
}

void	putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
		write(fd, s++, 1);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
		str[i++] = 0;
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*s;

	s = malloc(size * nmemb);
	if (!s)
		return (NULL);
	ft_bzero(s, (size * nmemb));
	return (s);
}
