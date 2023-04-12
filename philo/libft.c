/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-idri <bel-idri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:34:20 by bel-idri          #+#    #+#             */
/*   Updated: 2023/04/12 18:37:37 by bel-idri         ###   ########.fr       */
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

int	ft_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		ft_error("Error: gettimeofday failed\n");
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int time, int start)
{
	while (ft_get_time() - start < time)
		if (usleep(100))
			ft_error("Error: usleep failed\n");
}
