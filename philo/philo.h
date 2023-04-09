/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:43:29 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/07 16:01:57 by bel-idri         ###   ########.fr       */
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

	int nop;
	int tte;
	int tts;
	int ttd;
	int notepme;
	int notephe;
	int is_dead;
	pthread_mutex_t is_dead_mutex;
	pthread_mutex_t *forks;

}				t_data;

typedef struct s_philo
{
	pthread_t t;
	pthread_t d;
	int id;
	int start;
	int last_eat;
	t_data data;
	pthread_mutex_t last_eat_mutex;
	pthread_mutex_t print_msg_mutex;
}				t_philo;


#endif



// a.out // saug
// 1 parametre
