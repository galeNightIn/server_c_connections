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

#include "mldr_connect_client.h"

void *get_in_addr(struct sockaddr *sa) 
{ 
    if (sa->sa_family == AF_INET){ 
        return &(((struct sockaddr_in*)sa)->sin_addr); 
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr); 
}

int mldr_connect_to_client(int domain, int type, int flags, const char* port, const int backlog){
	
    int     rv = 0,
            sock_serv_fd = 0,
            reuse_addr = 1;

    struct addrinfo     serv_addr, *servinfo, *p;
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.ai_family = domain;
    serv_addr.ai_socktype = type;
    serv_addr.ai_flags = flags;

    if((rv = getaddrinfo(NULL, port, &serv_addr, &servinfo)) != 0){
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    for(p = servinfo; p != NULL; p = p->ai_next){
        
        if((sock_serv_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("socket");
            continue;
        }

        //reuse port
        if (setsockopt(sock_serv_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int)) == -1){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if(bind(sock_serv_fd, p->ai_addr, p->ai_addrlen) == -1){
            close(sock_serv_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if(p == NULL){
        fprintf(stderr, "S: error: could not bind to any address\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    if(listen(sock_serv_fd, backlog) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sock_serv_fd;
}