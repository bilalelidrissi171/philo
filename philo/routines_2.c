/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:39:11 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:39:56 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_take_forks(t_philo *philo)
{
	mutex_lock(&philo->data->forks[philo->id - 1]);
	ft_print_msg(philo, "has taken a fork\n");
	mutex_lock(&philo->data->forks[philo->id % philo->data->nop]);
	ft_print_msg(philo, "has taken a fork\n");
}

void	ft_dest_forks(t_philo *philo)
{
	mutex_unlock(&philo->data->forks[philo->id - 1]);
	mutex_unlock(&philo->data->forks[philo->id % philo->data->nop]);
}

void	ft_eating(t_philo *philo)
{
	mutex_lock(&philo->last_eat_mutex);
	philo->last_eat = ft_get_time();
	mutex_unlock(&philo->last_eat_mutex);
	mutex_lock(&philo->data->notephe_mutex);
	philo->data->notephe++;
	mutex_unlock(&philo->data->notephe_mutex);
	ft_print_msg(philo, "is eating\n");
	ft_usleep(philo->data->tte, ft_get_time());
}

void	ft_sleeping(t_philo *philo)
{
	ft_print_msg(philo, "is sleeping\n");
	ft_usleep(philo->data->tts, ft_get_time());
}

void	ft_thinking(t_philo *philo)
{
	ft_print_msg(philo, "is thinking\n");
}
