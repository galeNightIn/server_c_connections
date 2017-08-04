#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>

#define PORT 5000

#define BACKLOG 2

int main(int argc, char const *argv[])
{
	int sockfd = 0, //listened socket fd 
		newfd = 0,	//new socket fd
		rv = 0,
		reuse_addr = 1;

	struct addrinfo serv_addr, *servinfo, *p;
	struct sockaddr_storage clnt_addr;
	socketlen_t sin_size;

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.ai_family = AF_INET;
	serv_addr.ai_socktype = SOCK_STREAM;
	serv_addr.ai_flags = AI_PASSIVE;

	// customize servinfo
	if(rv = getaddrinfo(NULL, PORT, &serv_addr, &servinfo) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		
		if(sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1){
			fprintf(stderr, "socket: %s\n", strerror(errno));
			continue;
		}
	

		//reuse port
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int)) == -1){
 			perror("setsockopt");
 			exit(EXIT_FAILURE);
 		}	

 		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
 			close(sockfd);
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

 	if(listen(sockfd, BACKLOG) == -1){
 		perror("listen");
 		exit(EXIT_FAILURE)
 	}


	return 0;
}