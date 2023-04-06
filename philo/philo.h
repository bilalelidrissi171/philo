#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_data
{

	int nop; // number of philosophers
	int tte; // time to eat
	int tts; // time to sleep
	int ttd; // time to die
	int notepme; // number of time each philosopher must eat
	int notephe; // number of time each philosopher has eaten
	pthread_mutex_t *forks; // forks

}				t_data;

typedef struct s_philo
{
	pthread_t t;
	pthread_t f;
	pthread_t d;
	int id; // id of the philosopher
	int start; // time when the philosopher started
	int last_eat; // time when the philosopher last ate
	int is_dead; // is the philosopher dead
	t_data data; // data
	pthread_mutex_t last_eat_mutex; // eat
	pthread_mutex_t print_msg_mutex; // print message
	pthread_mutex_t is_dead_mutex; // is the philosopher dead
}				t_philo;


#endif



// a.out // saug
// last par = 0 // what I have to do
