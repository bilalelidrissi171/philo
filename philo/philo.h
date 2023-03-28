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

	int nop; // number of philosophers
	int tte; // time to eat
	int tts; // time to sleep
	int ttd; // time to die
	int notepme; // number of time each philosopher must eat
	pthread_mutex_t *forks; // forks

}				t_data;

typedef struct s_philo
{
	pthread_t t;
	pthread_t d;
	int id; // id of the philosopher
	int start; // time when the philosopher started
	int last_eat; // time when the philosopher last ate
	int is_dead; // is dead
	t_data data; // data
	pthread_mutex_t *left_fork; // left fork
	pthread_mutex_t *right_fork; // right fork
	pthread_mutex_t eat; // eat
	pthread_mutex_t print_msg; // print
	pthread_mutex_t notepme_mutex; // number of time each philosopher must eat
	pthread_mutex_t is_dead_mutex; // is dead
}				t_philo;


#endif
