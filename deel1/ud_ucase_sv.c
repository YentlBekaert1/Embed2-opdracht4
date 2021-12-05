#include "PJ_RPI.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include "tlpi_hdr.h"
#include <pthread.h>
#include "ud_ucase.h"

int state = 0;
int received_data_int;
int IO;
int period;
struct sockaddr_un svaddr, claddr;
int sfd, j;
ssize_t numBytes;
socklen_t len;
char buf[BUF_SIZE];
t_data received_data;



struct itimerval itv;
struct sigaction sa;

static void 
sigalrmHandler(int sig)
{
    t_data send_data;
	if (received_data_int == 1)
    {
          
        if(state == 0){
            GPIO_SET = (1 << IO) ^ GPIO_SET;
            state = 1;
            send_data.IO = IO;
            send_data.period = state;
            if (sendto(sfd, &send_data, sizeof(send_data), 0, (struct sockaddr *) &claddr, len) != sizeof(send_data))
                fatal("sendto"); 
        }else{
            GPIO_CLR = 1 << IO;
            state = 0;
            send_data.IO = IO;
            send_data.period = state;
            if (sendto(sfd, &send_data, sizeof(send_data), 0, (struct sockaddr *) &claddr, len) != sizeof(send_data))
                fatal("sendto");
        }
    }
    
   		
}

static void *
threadFunc(void *arg)
{
     
    len = sizeof(struct sockaddr_un);
    numBytes = recvfrom(sfd, &received_data, sizeof(received_data), 0, (struct sockaddr *) &claddr, &len);

    if (numBytes == -1)
        errExit("recvfrom");

    printf("Received %d: %d", received_data.IO, received_data.period);
        
    received_data_int = 1;
    period = received_data.period;
    IO = received_data.IO;
    printf("%d", IO);
    OUT_GPIO(received_data.IO);

    itv.it_value.tv_sec=received_data.period;
    itv.it_value.tv_usec =  0;
    itv.it_interval.tv_sec = received_data.period;
    itv.it_interval.tv_usec = 0;

    return "";
}




int main()
{
	sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrmHandler;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        errExit("sigaction");

	if(map_peripheral(&gpio) == -1) 
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    }

    
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


    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
    if (s != 0)
        errExitEN(s, "pthread_create");

    printf("Thread aangemaakt\n");

    s = pthread_join(t1, &res);
   
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Thread returned %ld \n", (long) res);

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1)
        errExit("setitimer");

    while (1){

    }
    
	return 0;	

}