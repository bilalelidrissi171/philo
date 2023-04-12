/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:43:25 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:39:54 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int		i;
	t_data	data;
	t_philo	*philo;

	ft_parsing(argc, argv, &data);
	philo = (t_philo *)ft_calloc(sizeof(t_philo), data.nop);
	ft_init(philo, &data);
	i = -1;
	while (++i < data.nop)
	{
		if (pthread_create(&philo[i].t, NULL, &ft_philo, &philo[i]))
			ft_free(philo, &data, 1, "Error: pthread_create error\n");
	}
	if (ft_check_death(philo, &data))
	{
		ft_free(philo, &data, 0, NULL);
		exit(0);
	}
	i = -1;
	while (++i < data.nop)
	{
		if (pthread_join(philo[i].t, NULL))
			ft_free(philo, &data, 1, "Error: pthread_join error\n");
	}
	return (ft_free(philo, &data, 0, NULL), 0);
}
