#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data
{

	size_t nop; // number of philosophers
	size_t tte; // time to eat
	size_t tts; // time to sleep
	size_t ttd; // time to die
	size_t notepme; // number of time each philosopher must eat
	size_t notephe; // number of time each philosopher has eaten
	pthread_mutex_t *forks; // forks

}				t_data;

typedef struct s_philo
{
	pthread_t t;
	pthread_t d;
	size_t id; // id of the philosopher
	size_t start; // time when the philosopher started
	size_t last_eat; // time when the philosopher last ate
	t_data data; // data
	pthread_mutex_t *left_fork; // left fork
	pthread_mutex_t *right_fork; // right fork
	pthread_mutex_t eat; // eat
	pthread_mutex_t print_msg; // print
	pthread_mutex_t is_dead; // is dead

}				t_philo;


#endif
