/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protiction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:35:21 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:35:26 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex))
		ft_error("Error: pthread_mutex_unlock failed\n");
}

void	mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex))
		ft_error("Error: pthread_mutex_lock failed\n");
}

void	mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr))
		ft_error("Error: pthread_mutex_init failed\n");
}

void	detach(pthread_t t)
{
	if (pthread_detach(t))
		ft_error("Error: pthread_detach failed\n");
}
