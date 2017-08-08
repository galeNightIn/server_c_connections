#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

#include "mldr_send_recv.h"
#include "mldr_connections.h"

#define PORT "5000"  

#define PORT_NEW "4500" 

#define BACKLOG 2 

#define MAXDATASIZE 100      


int main(int argc, char *argv[]) 
{   
    int sock_clnt_fd, numbytes; 
    char buf[MAXDATASIZE]; 
     
    int rv; 
   
    char cmd[MAXDATASIZE] = " ";

    if (argc != 2) { 
        fprintf(stderr,"usage: client hostname\n"); 
        exit(EXIT_FAILURE); 
    }

    sock_clnt_fd = mldr_connect_to_server(AF_UNSPEC, SOCK_STREAM, argv[1], PORT);

    while(strcmp(cmd, "exit") != 0){
        printf("-->");
        scanf("%s", cmd);
        printf("%s\n", cmd);
        int len = strlen(cmd);
        mldr_send(sock_clnt_fd, cmd, len, 0);
    }
    
    printf("client: received ’%s’\n",buf);

    close(sock_clnt_fd);

    return 0; 
} 
