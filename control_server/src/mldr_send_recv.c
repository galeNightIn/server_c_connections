#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h> 
#include <stdio.h> 

#include "mldr_send_recv.h"

void mldr_send(int sockfd, const char *buf, size_t len, int flags){
	
    if (send(sockfd, buf, len, flags) == -1) perror("send"); 

}

void mldr_recv(int sockfd, char *buf, size_t len, int flags, int *numbytes){
	
    int nb = 0;
    if ((nb = recv(sockfd, buf, len, flags)) == -1) { 
        perror("SEND: failed to recive buffer"); 
    } 

    buf[nb] = '\0';
    *numbytes = nb;

}
