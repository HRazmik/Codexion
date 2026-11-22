/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 14:25:01 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 18:09:09 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int  is_stopped(t_sim *sim)
{
    int s;

    pthread_mutex_lock(&sim->stop_mutex);
    s = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);
    return (s);
}

void    sim_stop(t_sim *sim)
{
    int i;

    pthread_mutex_lock(&sim->stop_mutex);
    sim->stop = 1;
    pthread_mutex_unlock(&sim->stop_mutex);

    i = 0;
    while (i < sim->args->n_of_coders)
    {
        pthread_mutex_lock(sim->dongles[i].mutex);
        pthread_cond_broadcast(sim->dongles[i].cond);
        pthread_mutex_unlock(sim->dongles[i]->mutex);
        i++;
    }
}

void *supervisor_function(void *arg)
{
	t_coder		*coders;
	int			i;
	int			num_coders;
	int			*burnout_flag;
	int			burnout_time;

	coders = (t_coder *)arg;
	num_coders = coders[0].args->n_of_coders;
	burnout_flag = coders[0].sim->stop;
	burnout_time = coders[0].args->t_to_burnout;
	while (!(*burnout_flag))
	{
		i = 0;
		while (i < num_coders)
		{
			if (now_ms(coders[i].time_last_comp) > burnout_time)
			{
				*burnout_flag = 1;
				pthread_cond_broadcast(&(coders[i].left->cond));
				printf("%ld %d burned out\n",
					now_ms(coders[i].time_of_start), coders[i].id);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
