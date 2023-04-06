#include "philo.h"

size_t	ft_strlen(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_error(char *str)
{
	write(2, "Error: ", 7);
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
		ft_error("malloc failed\n");
	memset(p, 0, count * size);
	if (!p)
		ft_error("memset failed\n");
	return (p);
}

void ft_free_exit(t_philo *philo, t_data *data)
{
	size_t i;

	i = 0;
	while (i < data->nop)
	{
		pthread_mutex_destroy(&philo[i].last_eat_mutex);
		pthread_mutex_destroy(&philo[i].print_msg_mutex);
		pthread_mutex_destroy(&philo[i].is_dead_mutex);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	exit(0);
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
			ft_error("negative number\n");
	while (str[i] >= '0' && str[i] <= '9')
	{
		temp = res;
		res = (res * 10) + (str[i++] - 48);
		if (temp != res / 10)
			ft_error("overflow\n");
	}
	if (str[i])
		ft_error("not a number\n");
	return (res);
}


void	ft_parsing(int argc, char **argv, t_data *data)
{
	if (argc < 5 && argc > 6)
		ft_error("wrong number of arguments\n");
	data->nop = ft_atoi(argv[1]);
	data->tte = ft_atoi(argv[2]);
	data->tts = ft_atoi(argv[3]);
	data->ttd = ft_atoi(argv[4]);
	data->notepme = 0;
	data->notephe = 0;
	if (argc == 6)
	{
		data->notepme = ft_atoi(argv[5]);
		if (data->notepme == 0)
			ft_error("number of time each philosopher must eat must be at least 1\n");
	}
	if (data->nop == 0)
		exit(0);
	if (data->nop > 200)
		ft_error("number of philosophers must be at most 200\n");
	if (data->tte < 60 || data->tts < 60 || data->ttd < 60)
		ft_error("time must be at least 60ms\n");
}

void	ft_init(t_philo *philo, t_data *data)
{
	size_t i;

	data->forks = ft_calloc(sizeof(pthread_mutex_t), data->nop);
	i = 0;
	while (i < data->nop)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&philo[i].last_eat_mutex, NULL);
		pthread_mutex_init(&philo[i].print_msg_mutex, NULL);
		pthread_mutex_init(&philo[i].is_dead_mutex, NULL);
		philo[i].id = i + 1;
		philo[i].start = 0;
		philo[i].last_eat = 0;
		philo[i].is_dead = 0;
		philo[i].data = *data;
		i++;
	}

}

int	ft_get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int time, int start)
{
	while (ft_get_time() - start < time)
		usleep(100);
}


void ft_print_msg(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->print_msg_mutex);
	printf("%d %d %s", ft_get_time() - philo->start, philo->id, str);
	pthread_mutex_unlock(&philo->print_msg_mutex);
}

void	ft_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data.forks[philo->id - 1]);
	ft_print_msg(philo, "has taken a fork\n");
	pthread_mutex_lock(&philo->data.forks[philo->id % philo->data.nop]);
	ft_print_msg(philo, "has taken a fork\n");
}

void	ft_dest_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data.forks[philo->id - 1]);
	pthread_mutex_unlock(&philo->data.forks[philo->id % philo->data.nop]);
}

void ft_eating(t_philo *philo)
{

	pthread_mutex_lock(&philo->last_eat_mutex);
	philo->last_eat = ft_get_time();
	philo->data.notephe++;
	ft_print_msg(philo, "is eating\n");
	if (philo->data.notepme)
	{
		if (philo->data.notephe == philo->data.notepme)
		{
			ft_dest_forks(philo);
			exit(0);
		}
	}
	ft_usleep(philo->data.tte, ft_get_time());
	pthread_mutex_unlock(&philo->last_eat_mutex);

}

void ft_sleeping(t_philo *philo)
{
	ft_print_msg(philo, "is sleeping\n");
	ft_usleep(philo->data.tts, ft_get_time());
}

void ft_thinking(t_philo *philo)
{
	ft_print_msg(philo, "is thinking\n");
}

void	*ft_philo(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	philo->start = ft_get_time();
	philo->last_eat = philo->start;
	if (philo->id % 2 == 0)
		usleep(100);
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


int	main(int argc, char **argv)
{
	size_t i;
	t_data data;
	t_philo philo[1064];

	ft_parsing(argc, argv, &data);

	// philo = (t_philo *)ft_calloc(sizeof(t_philo), data.nop);

	ft_init(philo, &data);



	i = 0;
	while (i < data.nop)
	{
		if (pthread_create(&philo[i].t, NULL, &ft_philo, &philo[i]))
			ft_error("pthread_create error\n");
		i++;
	}

	i = 0;
	while (i < data.nop)
	{
		if (pthread_join(philo[i].t, NULL))
			ft_error("pthread_join error\n");
		i++;
	}
	return (0);
}


