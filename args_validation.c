/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 15:17:21 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/19 20:12:22 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	str_to_int(char *nptr)
{
	int		i;
	int		number;

	i = 0;
	number = 0;
	while (nptr[i])
	{
		if (nptr[i] < '0' || nptr[i] > '9')
			return (-1);
		number = number * 10 + (nptr[i] - '0');
        if (number > INT_MAX)
            return (-1);
		i++;
	}
	return (number);
}

void	arr_to_args(int *arr, t_args *args)
{
	args->n_of_coders = arr[0];
	args->t_to_burnout = arr[1];
	args->t_to_compile = arr[2];
	args->t_to_debug = arr[3];
	args->t_to_refactor = arr[4];
	args->n_of_compiles_required = arr[5];
	args->time_from_start = start_timeval();
	args->dongle_cooldown = arr[6];
}

int	args_vallidation(int argc, char *argv[], t_args *args)
{
	int	i;
	int	arr[7];
	int	scheduler;
	int	tmp;

	if (argc != 9)
		return (1);
	scheduler = scheduler_validation(argv[8]);
	if (scheduler == -1)
		return (write(2, "Invalid scheduler\n", 18), 1);
	i = 1;
	args->scheduler = scheduler;
	while (i < 8)
	{
		tmp = str_to_int(argv[i]);
		if (tmp == -1)
			return (1);
		arr[i - 1] = tmp;
		i++;
	}
	arr_to_args(arr, args);
	return (0);
}

int	scheduler_validation(char *scheduler)
{
	if (strcmp(scheduler, "fifo") == 0)
		return (FIFO);
	else if (strcmp(scheduler, "edf") == 0)
		return (EDF);
	else
		return (-1);
}
