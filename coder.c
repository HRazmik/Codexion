/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 17:32:57 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 17:03:51 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
int	init_coders(t_coder	**coders, t_args *args, t_dongle **dongles)
{
	struct timeval time;
	int				num_coders;
	int				i;

	time = start_timeval();
	num_coders = args->n_of_coders;
	coders = malloc(sizeof(t_coder) * num_coders);
	if (!coders)
		return (1);
	i = 0;
	while (i < num_coders)
	{
		coders[i]->id = i + 1;
		coders[i]->args = args;
		coders[i]->time_last_comp = time;
		pass_dongles_to_coder(coders[i], dongles, num_coders, i);
		coders[i]->state = WORKING;
		i++;
	}
	return (0);
}

void pass_dongles_to_coder(t_coder *coder, t_dongle **dongles, int N, int i)
{
	int	a;
	int	b;

	a = i;
	b = i < (i + 1) % N;
	if (a < b)
	{
		coder->left = dongles[a];
		coder->right = dongles[b];
	}
	else
	{
		coder->left = dongles[b];
		coder->right = dongles[a];
	}
}

void	*coder_function(void *arg)
{
	int		count;
	t_coder	*coder;

	count = 0;
	coder = (t_coder *)arg;
	while (count < coder->args->n_of_compiles_required)
	{
		if(is_stopped(coder->sim))
			return (NULL);
		thread_coding(coder, coder->sim);
		count++;
	}
	write(1, "Coder finished\n", 16);
	coder->state = COMPLETE;
	return (NULL);
}

void	thread_coding(t_coder *c, t_sim *sim)
{
	if (!coder_take_dongles(c, sim))
		return ;
    pthread_mutex_lock(&sim->stop_mutex);
    c->time_last_comp = start_timeval();
    pthread_mutex_unlock(&sim->stop_mutex);
    log_state(sim, c->id, "is compiling");
	usleep(sim->args->t_to_compile * 1000);
    dongle_release(c->left, sim->args->dongle_cooldown);
    dongle_release(c->right, sim->args->dongle_cooldown);
	if (is_stopped(sim))
		return ;
    log_state(sim, c->id, "is debugging");
    usleep(c->args->t_to_debug * 1000);
    if (is_stopped(sim))
        return ;
    log_state(sim, c->id, "is refactoring");
    usleep(sim->args->t_to_refactor * 1000);
}

void	log_message(t_sim *sim, int id, const char *log)
{
	long	time;

	time = now_ms(sim->time_of_start);
	printf("%ld %d %s", time, id, log);	
}

/* returns 1 if the coder holds both dongles, 0 if stopped */
