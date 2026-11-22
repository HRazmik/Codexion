/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 12:48:10 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/20 14:44:47 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void wait_until(t_dongle *d, long until_ms)
{
    struct timespec ts;

    ts.tv_sec = until_ms / 1000;
    ts.tv_nsec = (until_ms % 1000) * 1000000L;
    pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
}

/* returns 1 if the coder holds the dongle, 0 if the simulation stopped */
int dongle_acquire(t_dongle *d, t_sim *sim)
{
    unsigned long   my;

    pthread_mutex_lock(&d->mutex);
    my = d->next_ticket++;
    while (!is_stopped(sim) && (d->held || my != d->now_serving))
        pthread_cond_wait(&d->cond, &d->mutex);
    while (!is_stopped(sim) && get_time_ms() < d->available_at)
        wait_until(d, d->available_at);
    if (is_stopped(sim))
    {
        pthread_mutex_unlock(&d->mutex);
        return (0);
    }
    d->held = 1;
    d->now_serving++;
    pthread_mutex_unlock(&d->mutex);
    return (1);
}