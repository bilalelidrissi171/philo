/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:36:46 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:37:03 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_parsing(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		ft_error("Error: wrong number of arguments\n");
	data->nop = ft_atoi(argv[1]);
	data->ttd = ft_atoi(argv[2]);
	data->tte = ft_atoi(argv[3]);
	data->tts = ft_atoi(argv[4]);
	data->notepme = 0;
	data->notephe = 0;
	if (argc == 6)
	{
		data->notepme = ft_atoi(argv[5]);
		if (data->notepme == 0)
			ft_error("Error: \
number of time each philosopher must eat must be at least 1\n");
	}
	if (data->nop == 0)
		ft_error("Error: number of philosophers must be at least 1\n");
	if (data->nop > 200)
		ft_error("Error: number of philosophers must be at most 200\n");
	if (data->tte < 60 || data->tts < 60 || data->ttd < 60)
		ft_error("Error: time must be at least 60ms\n");
}

void	ft_init(t_philo *philo, t_data *data)
{
	int	i;

	data->forks = ft_calloc(sizeof(pthread_mutex_t), data->nop);
	i = 0;
	mutex_init(&data->print_msg_mutex, NULL);
	mutex_init(&data->notephe_mutex, NULL);
	while (i < data->nop)
	{
		mutex_init(&data->forks[i], NULL);
		mutex_init(&philo[i].last_eat_mutex, NULL);
		philo[i].id = i + 1;
		philo[i].start = ft_get_time();
		philo[i].last_eat = philo[i].start;
		philo[i].data = data;
		i++;
	}
}

void	ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	exit(1);
}

void	ft_free(t_philo *philo, t_data *data, int x, char *str)
{
	int	i;

	pthread_mutex_destroy(&data->print_msg_mutex);
	pthread_mutex_destroy(&data->notephe_mutex);
	i = 0;
	while (i < data->nop)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philo[i++].last_eat_mutex);
	}
	i = 0;
	while (i < data->nop)
		detach(philo[i++].t);
	free(data->forks);
	free(philo);
	if (x)
		ft_error(str);
}
