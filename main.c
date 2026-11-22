/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 15:17:28 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 16:58:39 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	int			i;
	t_args		args;
	pthread_t	*threads_arr;
	pthread_t	threads_supervisor;
	t_dongle	*dongles;
	t_coder		*coders;
	t_sim		*sim;
	i = 0;
	if (args_vallidation(argc, argv, &args))
		return (1);
	dongles = init_dongles(args.n_of_coders);
	coders = init_coders(&args, dongles);
	threads_arr = malloc(args.n_of_coders * sizeof(pthread_t));
	pthread_create(&threads_supervisor, NULL, supervisor_function, coders);
	while (i < args.n_of_coders)
	{
		pthread_create(&threads_arr[i], NULL, coder_function, &coders[i]);
		i++;
	}
	i = 0;
	while (i < args.n_of_coders)
	{
		pthread_join(threads_arr[i], NULL);
		i++;
	}
	pthread_join(threads_supervisor, NULL);
	printf("All threads have finished\n");
	destroy_dongles(args.n_of_coders, dongles);
	free(coders);
	free(threads_arr);
	return (0);
}
