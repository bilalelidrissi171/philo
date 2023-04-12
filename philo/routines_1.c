/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:39:24 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:39:34 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		if (usleep(100))
			ft_error("Error: usleep failed\n");
	while (1)
	{
		ft_take_forks(philo);
		ft_eating(philo);
		ft_dest_forks(philo);
		ft_sleeping(philo);
		ft_thinking(philo);
	}
	return (NULL);
}

int	ft_check_eat(t_data *data)
{
	mutex_lock(&data->notephe_mutex);
	if (data->notephe >= data->nop * data->notepme && data->notepme)
	{
		mutex_unlock(&data->notephe_mutex);
		mutex_lock(&data->print_msg_mutex);
		return (1);
	}
	mutex_unlock(&data->notephe_mutex);
	return (0);
}

int	ft_check_death(t_philo *philo, t_data *data)
{
	int	i;

	while (1)
	{
		if (ft_check_eat(data))
			return (1);
		i = 0;
		while (i < philo->data->nop)
		{
			mutex_lock(&philo[i].last_eat_mutex);
			if (ft_get_time() - philo[i].last_eat > philo->data->ttd)
			{
				ft_print_msg(&philo[i], "died\n");
				mutex_lock(&data->print_msg_mutex);
				mutex_unlock(&philo[i].last_eat_mutex);
				return (1);
			}
			mutex_unlock(&philo[i].last_eat_mutex);
			i++;
		}
		if (usleep(200))
			ft_error("Error: usleep failed\n");
	}
	return (0);
}

void	ft_print_msg(t_philo *philo, char *str)
{
	mutex_lock(&philo->data->print_msg_mutex);
	printf("%d %d %s", ft_get_time() - philo->start, philo->id, str);
	mutex_unlock(&philo->data->print_msg_mutex);
}
