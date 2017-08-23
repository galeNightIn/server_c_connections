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
#define BACKLOG 1
#define MAXDATASIZE 100


int main(int argc, char const *argv[])
{	
	int sock_srv_fd 		= 0,
		sock_srv_fd_port	= 0, 
		new_serv_fd 		= 0,
		new_serv_fd_port	= 0,
		numbytes 			= 0;

	struct sockaddr_storage 	clnt_addr, clnt_addr_port;
	socklen_t 					sin_size, sin_size_port; 

	char s[INET6_ADDRSTRLEN] = " ";
	char s_port[INET6_ADDRSTRLEN] = " ";
	char cmd[MAXDATASIZE] = " ";

	sock_srv_fd = mldr_connect_to_client(PF_INET, SOCK_STREAM, AI_PASSIVE, PORT, BACKLOG);
	sock_srv_fd_port = mldr_connect_to_client(PF_INET, SOCK_STREAM, AI_PASSIVE, PORT_NEW, BACKLOG);
 		printf("server: waiting for connections...\n");
 		//while(1){
			sin_size = sizeof clnt_addr; 
			sin_size_port = sizeof clnt_addr_port;
			
			new_serv_fd = accept(sock_srv_fd, (struct sockaddr *)&clnt_addr, &sin_size);
			new_serv_fd_port = accept(sock_srv_fd_port, (struct sockaddr *)&clnt_addr_port, &sin_size_port);
			
			if (new_serv_fd  == -1) { 
				perror("Accept"); 
				//continue; 
   			}
   			
   			if (new_serv_fd_port  == -1) { 
				perror("Accept"); 
				//continue; 
   			}
   			inet_ntop(clnt_addr.ss_family, get_in_addr((struct sockaddr *)&clnt_addr), s, sizeof s);
	   		inet_ntop(clnt_addr_port.ss_family, get_in_addr((struct sockaddr *)&clnt_addr_port), s_port, sizeof s_port);
	   		printf("server: got connection from %s\n", s);	
    	    printf("server: got connection from %s\n", s_port);
    	    

    	    while(strcmp(cmd, "exit") != 0){
    	    	mldr_recv(new_serv_fd, cmd, MAXDATASIZE, 0, &numbytes);
    	    	printf("SERVER: recived %s message\n", cmd);
    	    	int len = strlen(cmd);
    	    	mldr_send(new_serv_fd_port, cmd, len, 0);
    	    	printf("SERVER: recived %s message\n", cmd);
    	    }
			close(new_serv_fd);
			close(new_serv_fd_port);
			
		//}   
		close(sock_srv_fd);
		close(sock_srv_fd_port);
	
	return 0;
}