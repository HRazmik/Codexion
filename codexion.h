/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rovnania <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 15:20:58 by rovnania          #+#    #+#             */
/*   Updated: 2026/11/22 17:37:35 by rovnania         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	FIFO,
	EDF,
}					t_scheduler;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				held;
	long			available_at;
	unsigned long	next_ticket;
	unsigned long	now_serving;
}					t_dongle;

typedef struct s_sim
{
	t_args			*args;
	t_dongle		**dongles;
	t_coder			**coders;
	int				stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
	struct timeval	time_of_start;
}					t_sim;

typedef struct s_args
{
	int				n_of_coders;
	int				t_to_burnout;
	int				t_to_compile;
	int				t_to_debug;
	int				t_to_refactor;
	int				n_of_compiles_required;
	int				dongle_cooldown;
	struct timeval	time_from_start;
	t_scheduler		scheduler;
}					t_args;

typedef enum e_coder_state
{
	WORKING,
	COMPLETE,
	FINISHED
}					t_coder_state;

typedef struct s_coder
{
	int				id;
	t_args			*args;
	t_sim			*sim;
	struct timeval	time_last_comp;
	t_dongle		*left;
	t_dongle		*right;
	t_coder_state	state;
}					t_coder;

struct timeval		time_timeval(struct timeval start_time);
long				to_ms(struct timeval tv);
long				now_ms(struct timeval tv);
struct timeval		start_timeval(void);
void				thread_coding(t_coder *coder, t_sim *sim);
int					init_dongles(t_dongle **dongles, int num_dongles);
int					destroy_dongles(int num_dongles, t_dongle *dongles);
void				*coder_function(void *arg);
void				*supervisor_function(void *arg);
int					args_vallidation(int argc, char *argv[], t_args *args);
void				arr_to_args(int *arr, t_args *args);
int					str_to_int(char *nptr);
int					scheduler_validation(char *scheduler);
void				log_message(t_sim *sim, int id, const char *log);
void				pass_dongles_to_coder(t_coder *coder, t_dongle **dongles,
						int N, int i);

#endif
