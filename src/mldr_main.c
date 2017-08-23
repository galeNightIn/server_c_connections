#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h> 
#include <sys/wait.h> 
#include <signal.h>

#include "mldr_send_recv.h"
#include "mldr_connections.h"

#define PORT "5000"

#define PORT_NEW "4500"

#define BACKLOG 2

#define MAXDATASIZE 100


int main(int argc, char const *argv[])
{	
	int sock_srv_fd 	= 0, 
		new_serv_fd 	= 0,
		numbytes 		= 0;

	struct sockaddr_storage 	clnt_addr;
	socklen_t 					sin_size; 

	char s[INET6_ADDRSTRLEN];
	char cmd[MAXDATASIZE] = " ";

	sock_srv_fd = mldr_connect_to_client(AF_INET, SOCK_STREAM, AI_PASSIVE, PORT, BACKLOG);

 	printf("server: waiting for connections...\n");

 	while(1){

 		sin_size = sizeof clnt_addr; 
		new_serv_fd = accept(sock_srv_fd, (struct sockaddr *)&clnt_addr, &sin_size); 
		
		if (new_serv_fd == -1) { 
			perror("accept"); 
			continue; 
   		}

   		inet_ntop(clnt_addr.ss_family, get_in_addr((struct sockaddr *)&clnt_addr), s, sizeof s);

        printf("server: got connection from %s\n", s);	

 		if (!fork()) {   
    	    close(sock_srv_fd);  
    	    while(strcmp(cmd, "exit") != 0){
    	    	mldr_recv(new_serv_fd, cmd, MAXDATASIZE, 0, &numbytes);
    	    	printf("SERVER: recived %s message\n", cmd);
    	    }
			close(new_serv_fd); 
			exit(0); 
		}
			
		close(new_serv_fd);   
 	}

		
	return 0;
}
