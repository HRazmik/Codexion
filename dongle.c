/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 14:39:10 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 17:46:16 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_dongle	**dongles, int num_dongles)
{
	int			i;

	dongles = (t_dongle *)malloc(sizeof(t_dongle) * num_dongles);
	if (!dongles)
		return (1);
	i = 0;
	while (i < num_dongles)
	{
		pthread_mutex_init(&(dongles[i]->mutex), NULL);
		pthread_cond_init(&(dongles[i]->cond), NULL);
		dongles[i]->held = 0;
		dongles[i]->available_at = 0;
		dongles[i]->next_ticket = 0;
		dongles[i]->now_serving = 0;
		i++;
	}
	return (0);
}

int	destroy_dongles(int num_dongles, t_dongle *dongles)
{
	int	i;

	i = 0;
	if (!dongles)
		return (write(1, "ERROR\n", 6), 0);
	while (i < num_dongles)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	free(dongles);
	dongles = 0;
	return (1);
}

int coder_take_dongles(t_coder *c, t_sim *sim)
{
    if (!dongle_acquire(c->left, sim))
        return (0);
    log_message(sim, c->id, "has taken a dongle");
    if (!dongle_acquire(c->right, sim))
    {
		log_message(sim, c->id, "has released a dongle");
        dongle_release(c->left, sim->args->dongle_cooldown);
        return (0);
    }
    log_message(sim, c->id, "has taken a dongle");
    return (1);
}

void    dongle_release(t_dongle *d, t_sim *sim)
{
    pthread_mutex_lock(&d->mutex);
    d->held = 0;
    d->available_at = now_ms(sim->time_of_start) + sim->args->dongle_cooldown;
    pthread_cond_broadcast(&d->cond);
    pthread_mutex_unlock(&d->mutex);
}