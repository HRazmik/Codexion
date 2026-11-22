/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/20 17:15:39 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 17:13:31 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sim(t_sim *sim, t_args *args)
{	
	sim->args = args;
	pthread_mutex_init(&sim->stop_mutex, NULL);
	pthread_cond_init(&sim->log_mutex, NULL);
	gettimeofday(&sim->time_of_start, NULL);
	if (init_dongles(sim->dongles, args->n_of_coders))
		return(write(1, "ERROR\n", 6), 1);
	if (init_coders(sim->coders, args, sim->dongles))
		return(write(1, "ERROR\n", 6), 1);
	sim->stop = 0;

	return (0);
}
