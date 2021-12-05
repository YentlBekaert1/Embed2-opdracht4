/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 57-6 */

/* ud_ucase_sv.c

   A server that uses a UNIX domain datagram socket to receive datagrams,
   convert their contents to uppercase, and then return them to the senders.

   See also ud_ucase_cl.c.
*/
#include "ud_ucase.h"
#include "PJ_RPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{
    if(map_peripheral(&gpio) == -1) 
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    }

    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    /* Create server socket */
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);       
    if (sfd == -1)
        errExit("socket");

    if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1)
        fatal("Server socket path too long: %s", SV_SOCK_PATH);

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
        errExit("remove-%s", SV_SOCK_PATH);

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    /* Receive messages, convert to uppercase, and return to client */
    int received_data_int;
    int period;
    for(;;){
        t_data received_data;
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, &received_data, sizeof(received_data), 0, (struct sockaddr *) &claddr, &len);

        if (numBytes == -1)
            errExit("recvfrom");

        printf("Received %d: %d", received_data.IO, received_data.period);
        
        if (sendto(sfd, &received_data, sizeof(received_data), 0, (struct sockaddr *) &claddr, len) != numBytes)
            fatal("sendto"); 

        OUT_GPIO(received_data.IO);

            GPIO_SET = (1<<17) ^ GPIO_SET;
            sleep(received_data.period);
            GPIO_CLR = 1 << 17;
		    sleep(received_data.period);

    }
}

/*
        OUT_GPIO(received_data.IO);
        */