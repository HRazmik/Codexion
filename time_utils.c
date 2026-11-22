/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 15:03:14 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/20 12:11:28 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
struct timeval	time_timeval(struct timeval start_time)
{
	struct timeval	tv;
	struct timeval	present_time;
	if (gettimeofday(&tv, NULL) == -1)
		return (write(2, "Error__1", 8), start_time);
	present_time.tv_sec = tv.tv_sec - start_time.tv_sec;
	present_time.tv_usec = tv.tv_usec - start_time.tv_usec;
	return (present_time);
}

long	to_ms(struct timeval tv)
{
	long	msec;
	msec = tv.tv_sec * 1000;
	msec += tv.tv_usec / 1000L;
	return (msec);
}

long    get_time_ms(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long	now_ms(struct timeval tv)
{
	long	msec;
	struct timeval tmp_tv;
	tmp_tv = time_timeval(tv);
	msec = tmp_tv.tv_sec * 1000;
	msec += tmp_tv.tv_usec / 1000L;
	return (msec);
}
struct timeval	start_timeval(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv);
}