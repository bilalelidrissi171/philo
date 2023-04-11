/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:43:25 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/11 21:37:14 by bel-idri         ###   ########.fr       */
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

void	mutex_destroy(pthread_mutex_t *mutex)
{
	if (pthread_mutex_destroy(mutex))
		ft_error("Error: pthread_mutex_destroy failed\n");
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

void	ft_free(t_philo *philo, t_data *data, int x, char *str)
{
	int	i;

	i = 0;
	mutex_unlock(&data->print_msg_mutex);
	mutex_unlock(&data->notephe_mutex);
	mutex_destroy(&data->print_msg_mutex);
	mutex_destroy(&data->notephe_mutex);
	while (i < data->nop)
	{
		mutex_unlock(&data->forks[i]);
		mutex_unlock(&philo[i++].last_eat_mutex);
	}
	i = 0;
	while (i < data->nop)
	{
		mutex_destroy(&data->forks[i]);
		mutex_destroy(&philo[i++].last_eat_mutex);
	}
	i = 0;
	while (i < data->nop)
		detach(philo[i++].t);
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

	if (gettimeofday(&tv, NULL))
		ft_error("Error: gettimeofday failed\n");
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

void	ft_usleep(int time, int start)
{
	while (ft_get_time() - start < time)
		if (usleep(100))
			ft_error("Error: usleep failed\n");
}

void	ft_print_msg(t_philo *philo, char *str)
{
	mutex_lock(&philo->data->print_msg_mutex);
	printf("%d %d %s", ft_get_time() - philo->start, philo->id, str);
	mutex_unlock(&philo->data->print_msg_mutex);
}

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

int	ft_check_notephe(t_data *data)
{
	mutex_lock(&data->notephe_mutex);
	if (data->notephe >= data->nop * data->notepme && data->notepme)
	{
		mutex_unlock(&data->notephe_mutex);
		return (1);
	}
	mutex_unlock(&data->notephe_mutex);
	return (0);
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
	return (ft_free(philo, &data, 0, NULL), 0);
}
