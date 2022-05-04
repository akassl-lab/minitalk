/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmallett <nmallett@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 11:50:23 by nmallett          #+#    #+#             */
/*   Updated: 2022/04/28 13:57:29 by nmallett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "libft/libft.h"

typedef struct t_store_bit_data
{
	int		i[2];
	char	c;
}	t_store_bit_data;

/*
*	When creating the signals you must always have a handle within
*	the signal function, in this case we are making sure that the user 
*	defined signal is is being used and calculating the the amount of
*	signals until the signal is no longer
*	a user defined signal or is NUll. If the signal is not a userdefined 
*	or null, print a 0 meaning the client received a signal from
*	the server side confirming the client signal that was sent was received 
*	and processed (BONUS)
*/
void	handle_sigusr(int signum)
{
	int	i;

	i = 0;
	if (signum == SIGUSR1 && signum != 0)
		++i;
	else
	{
		ft_printf("%d\n", i);
		exit(0);
	}
}

/*
*	Explaination of our we are handling our string to the server
*	Example of right shifting 4 arithmetic positions 
*	(Condition: -2147483552 >> 4)
*	Before: 10000000 00000000 00000000 01100000
*	After: 	11111000 00000000 00000000 00000110
*	By shifting the binary bits by 4 and making sure 
*	our binary digit is 'One' and not a 'Zero' Since we want to forward 
*	valid binary data to the server to let the 
*	server translate our string. You cannot forward data 
*	in signals and you have no choice to only forward it using binary 
*	and slim it down with bits. Once you have the current bit to 
*	send over and it has been shifted within the binary pads, 
*	you may send the signal.  Remember you can use used defines signals 
*	like SIGUSR1 and SIGUSR2 to send over the 
*	data to our server handler. Another note: Remember to always reset 
*	your current byte data when creating a new loop to set it back to the size
*	 of 1 byte that is 8. For the (Bonus Part): 
*	Since we are sending over the string as bits, we are able to read UNICODE 
*	strings since UNICODE is 1-4 bytes. 
*	Send your best emoji during the evaluation ;)
*/
void	send_string_as_bits(int pid, char *str)
{
	t_store_bit_data	stack;

	stack.i[0] = 0;
	while (str[stack.i[0]] != '\0')
	{	
		stack.i[1] = 8;
		stack.c = str[stack.i[0]++];
		while (stack.i[1]--)
		{
			if (stack.c >> stack.i[1] & 1)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
			usleep(100);
		}
	}
	stack.i[1] = 8;
	while (stack.i[1]--)
	{
		kill(pid, SIGUSR1);
		usleep(100);
	}
}

/*
*	Our program main, check if the required arguments are present or return it
*	Once we pass the verification, print the client process ID and 
*	print the Server process ID that was specified
*	Create both of our USER Defines signals through 
*	our handler and send our string bit by bit.
*/
int	main(int argc, char *argv[])
{
	if (argc != 3)
	{
		ft_printf("MINITALK (42-Quebec)\n\nSYNTAX: /client [PID] [STRING]\n");
		return (-1);
	}
	ft_printf("Current Client PID: %d\n", getpid());
	ft_printf("Connected to Server PID: %d\n", ft_atoi(argv[1]));
	ft_printf("Sending: %s\n", argv[2]);
	signal(SIGUSR1, handle_sigusr);
	signal(SIGUSR2, handle_sigusr);
	send_string_as_bits(ft_atoi(argv[1]), argv[2]);
	while (1)
		sleep (1);
	return (0);
}
