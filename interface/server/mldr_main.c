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

#define PORT "5000"

#define BACKLOG 2

#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa) 
{ 
    if (sa->sa_family == AF_INET){ 
        return &(((struct sockaddr_in*)sa)->sin_addr); 
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr); 
}



int main(int argc, char const *argv[])
{	
	int sock_fd 	= 0, 
		new_fd 		= 0,	
		rv 			= 0,
		reuse_addr 	= 1,
		numbytes 	= 0;

	struct addrinfo 			serv_addr, *servinfo, *p;
	struct sockaddr_storage 	clnt_addr;
	socklen_t 					sin_size; 

	char s[INET6_ADDRSTRLEN];
	char cmd[MAXDATASIZE] = " ";

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.ai_family 	= AF_INET;
	serv_addr.ai_socktype 	= SOCK_STREAM;
	serv_addr.ai_flags 		= AI_PASSIVE;

	
	if((rv = getaddrinfo(NULL, PORT, &serv_addr, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		
		if((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			fprintf(stderr, "socket: %s\n", strerror(errno));
			continue;
		}
	

		//reuse port
		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int)) == -1){
 			perror("setsockopt");
 			exit(EXIT_FAILURE);
 		}	

 		if(bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1){
 			close(sock_fd);
 			perror("server: bind");
 			continue;
 		}

 		break;
 	}
 	
 	if(p == NULL){
 		fprintf(stderr, "S: error: could not bind to any address\n");
        return EXIT_FAILURE;
 	}

 	freeaddrinfo(servinfo);

 	if(listen(sock_fd, BACKLOG) == -1){
 		perror("listen");
 		exit(EXIT_FAILURE);
 	}

 	printf("server: waiting for connections...\n");

 	while(1){

 		sin_size = sizeof clnt_addr; 
		new_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &sin_size); 
		
		if (new_fd == -1) { 
			perror("accept"); 
			continue; 
   		}

   		inet_ntop(clnt_addr.ss_family, get_in_addr((struct sockaddr *)&clnt_addr), s, sizeof s);

        printf("server: got connection from %s\n", s);	

 		if (!fork()) {   
    	    close(sock_fd);  
    	    
    	    while(strcmp(cmd, "exit") != 0){
    	    	mldr_recv(new_fd, cmd, MAXDATASIZE, 0, &numbytes);
    	    	printf("SERVER: recived %s message\n", cmd);
    	    }
    	    //mldr_send(new_fd, "Hello, world!", 13, 0);  
			

			mldr_send(sock_fd, "LOG: CHECK FROM SERVER", 25, 0);

			close(new_fd); 
			exit(0); 
		}

		close(new_fd);   
 	}

		
     


	return 0;
}