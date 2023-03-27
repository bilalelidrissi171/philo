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

size_t	ft_atoi(char *str)
{
	size_t	i;
	size_t	res;
	size_t	temp;

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
	if (data->nop > 200)
		ft_error("number of philosophers must be at most 200\n");
	data->tte = ft_atoi(argv[2]);
	if (data->tte < 60)
		ft_error("time to eat must be at least 60ms\n");
	data->tts = ft_atoi(argv[3]);
	if (data->tts < 60)
		ft_error("time to sleep must be at least 60ms\n");
	data->ttd = ft_atoi(argv[4]);
	if (data->ttd < 60)
		ft_error("time to die must be at least 60ms\n");
	data->notepme = 0;
	if (argc == 6)
	{
		data->notepme = ft_atoi(argv[5]);
		if (data->notepme == 0)
			ft_error("number of time each philosopher must eat must be at least 1\n");
	}
	if (data->nop == 0)
		exit(0);
}

void	ft_init(t_philo *philo, t_data *data)
{
	size_t i;

	i = 0;
	while (i < data->nop)
	{
		philo[i].id = i + 1;
		philo[i].start = 0;
		philo[i].last_eat = 0;
		philo[i].is_dead = 0;
		philo[i].data = *data;
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&philo[i].eat, NULL);
		pthread_mutex_init(&philo[i].print_msg, NULL);
		pthread_mutex_init(&philo[i].is_death, NULL);
		i++;
	}
	i = 0;
	while (i < data->nop)
	{
		if (i == 0)
		{
			philo[i].left_fork = &data->forks[data->nop - 1];
			philo[i].right_fork = &data->forks[i];
		}
		else if (i == data->nop - 1)
		{
			philo[i].left_fork = &data->forks[i];
			philo[i].right_fork = &data->forks[0];
		}
		else
		{
			philo[i].left_fork = &data->forks[i];
			philo[i].right_fork = &data->forks[i + 1];
		}
		i++;
	}

}

size_t	ft_get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(size_t time)
{
	size_t start;

	start = ft_get_time();
	while (ft_get_time() - start < time)
		usleep(100);
}


void	*ft_philo(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	philo->start = ft_get_time();
	philo->last_eat = philo->start;
	while (1)
	{
		if (philo->id % 2 == 0)
			ft_usleep(1);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->print_msg);
		printf("%zu %zu has taken a fork\n", ft_get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print_msg);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->print_msg);
		printf("%zu %zu has taken a fork\n", ft_get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print_msg);
		pthread_mutex_lock(&philo->eat);
		philo->last_eat = ft_get_time();
		pthread_mutex_lock(&philo->print_msg);
		printf("%zu %zu is eating\n", ft_get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print_msg);
		ft_usleep(philo->data.tte);
		pthread_mutex_unlock(&philo->eat);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_lock(&philo->print_msg);
		printf("%zu %zu is sleeping\n", ft_get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print_msg);
		ft_usleep(philo->data.tts);
		pthread_mutex_lock(&philo->print_msg);
		printf("%zu %zu is thinking\n", ft_get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print_msg);
	}
	return (NULL);
}





int	main(int argc, char **argv)
{
	size_t i;
	t_data data;
	t_philo *philo;

	ft_parsing(argc, argv, &data);

	philo = (t_philo *)malloc(sizeof(t_philo) * data.nop);
	if (!philo)
		ft_error("malloc error\n");

	ft_init(philo, &data);

	i = 0;
	while (i < data.nop)
	{
		if (pthread_create(&philo[i].t, NULL, ft_philo, &philo[i]))
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


	free(philo);


	return (0);
}


