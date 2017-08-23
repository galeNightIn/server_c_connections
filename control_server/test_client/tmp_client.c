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
#include <pthread.h>

#include "mldr_send_recv.h"
#include "mldr_connect_server.h"

#define PORT "4500"  
#define PORT_NEW "5000" 
#define BACKLOG 1
#define MAXDATASIZE 100   
#define HOST "127.0.0.1"



int main(int argc, char *argv[]) 
{ 
	int 	sock_clnt_fd        = 0,
        	sock_clnt_fd_port   = 0,
        	numbytes            = 0,
        	numbytes_port       = 0; 
    
    char buf[MAXDATASIZE]; 
    char cmd[MAXDATASIZE];

 

    // sock_clnt_fd = mldr_connect_to_server(AF_UNSPEC, SOCK_STREAM, HOST, PORT);
    // sock_clnt_fd_port = mldr_connesct_to_server(AF_UNSPEC, SOCK_STREAM, HOST, PORT_NEW);
    

    sock_clnt_fd = mldr_connect_to_server(AF_UNSPEC, SOCK_STREAM, HOST, PORT);
    sock_clnt_fd_port = mldr_connect_to_server(AF_UNSPEC, SOCK_STREAM, HOST, PORT_NEW);
    
    sleep(2);
    
    while(strcmp(cmd, "exit") != 0){
        printf("-->");
        scanf("%s", cmd);
    
        int len = strlen(cmd);
        mldr_send(sock_clnt_fd, cmd, len, 0);
        printf("CLIENT: sent %s message\n", cmd);
        mldr_recv(sock_clnt_fd_port, buf, MAXDATASIZE, 0, &numbytes_port);
        printf("CLIENT: recv succes %s message\n\n", buf);
    }
    close(sock_clnt_fd);
    close(sock_clnt_fd_port);
       
    return 0; 
} 