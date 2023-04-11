/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:43:25 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/11 17:24:03 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_error(char *str)
{
	write(2, str, ft_strlen(str));
	exit(1);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	void	*p;

	i = -1;
	if (count)
		if (size >= i / count)
			return (NULL);
	if (size)
		if (count >= i / size)
			return (NULL);
	p = malloc(count * size);
	if (!p)
		ft_error("Error: malloc failed\n");
	memset(p, 0, count * size);
	if (!p)
		ft_error("Error: memset failed\n");
	return (p);
}

size_t	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	temp;

	res = 0;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			ft_error("Error: negative number or not a number\n");
	if (!str[i])
		ft_error("Error: not a number\n");
	while (str[i] >= '0' && str[i] <= '9')
	{
		temp = res;
		res = (res * 10) + (str[i++] - 48);
		if (temp != res / 10)
			ft_error("Error: overflow\n");
	}
	if (str[i])
		ft_error("Error: not a number\n");
	return (res);
}

void	ft_free(t_philo *philo, t_data *data, int x, char *str)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->print_msg_mutex);
	while (i < data->nop)
		pthread_mutex_destroy(&data->forks[i++]);
	i = 0;
	while (i < data->nop)
		pthread_mutex_destroy(&philo[i++].last_eat_mutex);
	i = 0;
	while (i < data->nop)
		pthread_detach(philo[i++].t);
	free(data->forks);
	free(philo);
	if (x)
		ft_error(str);
}

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

int	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_init(t_philo *philo, t_data *data)
{
	int	i;

	data->forks = ft_calloc(sizeof(pthread_mutex_t), data->nop);
	i = 0;
	pthread_mutex_init(&data->print_msg_mutex, NULL);
	while (i < data->nop)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&philo[i].last_eat_mutex, NULL);
		philo[i].id = i + 1;
		philo[i].start = ft_get_time();
		philo[i].last_eat = philo[i].start;
		philo[i].data = data;
		i++;
	}
}

void	ft_usleep(int time, int start)
{
	while (ft_get_time() - start < time)
		usleep(100);
}

void	ft_print_msg(t_philo *philo, char *str)
{
	if (pthread_mutex_lock(&philo->data->print_msg_mutex))
		ft_error("Error: mutex lock failed\n");
	printf("%d %d %s", ft_get_time() - philo->start, philo->id, str);
	if (pthread_mutex_unlock(&philo->data->print_msg_mutex))
		ft_error("Error: mutex unlock failed\n");
}

void	ft_take_forks(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->data->forks[philo->id - 1]))
		ft_error("Error: mutex lock failed\n");
	ft_print_msg(philo, "has taken a fork\n");
	if (pthread_mutex_lock(&philo->data->forks[philo->id % philo->data->nop]))
		ft_error("Error: mutex lock failed\n");
	ft_print_msg(philo, "has taken a fork\n");
}

void	ft_dest_forks(t_philo *philo)
{
	if (pthread_mutex_unlock(&philo->data->forks[philo->id - 1]))
		ft_error("Error: mutex unlock failed\n");
	if (pthread_mutex_unlock(&philo->data->forks[philo->id % philo->data->nop]))
		ft_error("Error: mutex unlock failed\n");
}

int	ft_check_notephe(t_data *data)
{

	if (data->notephe >= data->nop * data->notepme && data->notepme)
	{
		// if (pthread_mutex_lock(&data->print_msg_mutex))
		// 	ft_error("Error: mutex lock failed\n");
		return (1);
	}
	return (0);

}

void	ft_eating(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->last_eat_mutex))
		ft_error("Error: mutex lock failed\n");
	philo->last_eat = ft_get_time();
	philo->data->notephe++;
	ft_print_msg(philo, "is eating\n");
	if (pthread_mutex_unlock(&philo->last_eat_mutex))
		ft_error("Error: mutex unlock failed\n");
	ft_usleep(philo->data->tte, ft_get_time());
	// return (0);
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

void	*ft_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->last_eat_mutex);
	philo->start = ft_get_time();
	philo->last_eat = philo->start;
	pthread_mutex_unlock(&philo->last_eat_mutex);
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (ft_check_notephe(philo->data))
			return (NULL);
		ft_take_forks(philo);
		if (ft_check_notephe(philo->data))
			return (NULL);

		ft_eating(philo);
		if (ft_check_notephe(philo->data))
			return (NULL);
		ft_dest_forks(philo);
		if (ft_check_notephe(philo->data))
			return (NULL);
		ft_sleeping(philo);
		if (ft_check_notephe(philo->data))
			return (NULL);
		ft_thinking(philo);
		if (ft_check_notephe(philo->data))
			return (NULL);
	}
	return (NULL);
}

int	ft_check_death(t_philo	*philo, t_data *data)
{
	int i;

	i = 0;
	while (1)
	{
		if (data->notephe >= data->nop * data->notepme && data->notepme)
			return (0);
		i = 0;
		while (i < philo->data->nop)
		{
			if (pthread_mutex_lock(&philo[i].last_eat_mutex))
				ft_error("Error: mutex lock failed\n");
			if (ft_get_time() - philo[i].last_eat > philo->data->ttd)
			{
				ft_print_msg(&philo[i], "died\n");
				if (pthread_mutex_unlock(&philo[i].last_eat_mutex))
					ft_error("Error: mutex unlock failed\n");
				pthread_mutex_lock(&philo->data->print_msg_mutex);
				return (1);
			}
			if (pthread_mutex_unlock(&philo[i].last_eat_mutex))
				ft_error("Error: mutex unlock failed\n");
			i++;
		}
		usleep(200);
	}
	return (0);
}

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
		exit (0);
	}
	i = -1;
	while (++i < data.nop)
	{
		if (pthread_join(philo[i].t, NULL))
			ft_free(philo, &data, 1, "Error: pthread_join error\n");
	}
	ft_free(philo, &data, 0, NULL);
	return (0);
}
