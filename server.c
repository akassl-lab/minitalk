/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmallett <nmallett@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 11:50:50 by nmallett          #+#    #+#             */
/*   Updated: 2022/04/28 13:48:41 by nmallett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include "libft/libft.h"

typedef struct t_store_client_data
{
	pid_t			client_pid;
	unsigned char	c;
	int				i;
	bool			first_start;
}	t_store_client_data;

struct sigaction	g_s_structaction;
t_store_client_data	client_data;

/*
*	We are using the same process and structure as the client side except
*	this time we are listening for the signal and forwarding the specified bit
*	that was transferred with the signal. As we mentionned in our 
*	client side, we cannot transfer	an entire string or a specific character
*	we can only do it by swiftting over the pads of an 8 byte memory
*	and sending the data over using a user defined signal. 
*
*	Break down of our function: Get the context of the signal
*	the client information and compare it with our SIGUSR 
*	to make sure we are handling the right signal. 
*	Then send over the data to our stack and do the same workaround
*	as the client side to read it 8 bits at a time. 
*	The only difference, we dont need to create a loop like on the client side
*	since the server does not have a whole string but 
*	only 1 character that we must translate from bit forms
*/
void	server_handler(int signum, siginfo_t *client_info, void *context)
{
	(void)context;
	if (!client_data.client_pid)
		client_data.client_pid = client_info->si_pid;
	client_data.c |= (signum == SIGUSR2);
	if (++client_data.i == 8)
	{
		if (client_data.c == '\0')
			write(1, "\n", 2);
		client_data.i = 0;
		if (!client_data.c)
		{
			kill(client_data.client_pid, SIGUSR2);
			client_data.client_pid = 0;
			return ;
		}
		write(1, &client_data.c, 1);
		client_data.c = 0;
		kill(client_data.client_pid, SIGUSR1);
	}
	else
		client_data.c <<= 1;
}

/*
*	Our main for our server-side. As specified in the subjet
*	print the server process ID but I don't want to spam it every 100ms
*	Just create a boolean and process it once, don't need to 
*	know our PID every single time there's a new 
*	tick in the thread in our program.
*	
*	After get the default stack data that is included in the 
*	sigaction stack to define our server handle and any
*	context information that can be provided from the SIGINFO. 
*	Once the information is gathered, process with our sigaction function
* 	to start initalizing our listener for any signals related 
*	to our user defines signals and the data we specified in our stack
*	while all of that is going on, let the server-side sleep. 
*	Using microsecond interval seconds, will let the server 
*	handle any on-going spam messages to not have a delay 
*	in any of these messages. The best practice would be to create a queue 
*	system for any incoming signals to process them by the tick of the thread
*/
int	main(void)
{
	client_data.first_start = false;
	if (client_data.first_start == false)
	{
		client_data.first_start = true;
		ft_printf("Server PID: %d\n", getpid());
	}
	g_s_structaction.sa_sigaction = server_handler;
	g_s_structaction.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &g_s_structaction, 0);
	sigaction(SIGUSR2, &g_s_structaction, 0);
	while (1)
		usleep(100);
	return (0);
}
