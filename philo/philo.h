/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:43:29 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:44:50 by bel-idri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int				nop;
	int				tte;
	int				tts;
	int				ttd;
	int				notepme;
	int				notephe;
	pthread_mutex_t	print_msg_mutex;
	pthread_mutex_t	notephe_mutex;
	pthread_mutex_t	*forks;

}				t_data;

typedef struct s_philo
{
	pthread_t		t;
	int				id;
	int				start;
	int				last_eat;
	t_data			*data;
	pthread_mutex_t	last_eat_mutex;
}				t_philo;

size_t	ft_strlen(char *str);
void	*ft_calloc(size_t count, size_t size);
size_t	ft_atoi(char *str);
int		ft_get_time(void);
void	ft_usleep(int time, int start);
void	ft_parsing(int argc, char **argv, t_data *data);
void	ft_init(t_philo *philo, t_data *data);
void	ft_error(char *str);
void	ft_free(t_philo *philo, t_data *data, int x, char *str);
void	mutex_unlock(pthread_mutex_t *mutex);
void	mutex_lock(pthread_mutex_t *mutex);
void	mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
void	detach(pthread_t t);
void	*ft_philo(void *arg);
int		ft_check_eat(t_data *data);
int		ft_check_death(t_philo *philo, t_data *data);
void	ft_print_msg(t_philo *philo, char *str);
void	ft_take_forks(t_philo *philo);
void	ft_dest_forks(t_philo *philo);
void	ft_eating(t_philo *philo);
void	ft_sleeping(t_philo *philo);
void	ft_thinking(t_philo *philo);

#endif
