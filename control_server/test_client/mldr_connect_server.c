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

#include "mldr_connect_server.h"

void *get_in_addr(struct sockaddr *sa)
{ 
    if (sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int mldr_connect_to_server(int domain, int type, const char* node, const char* port){
    
    int     rv = 0,
            sock_clnt_fd = 0;
    
    struct addrinfo     hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family 	= domain;
    hints.ai_socktype 	= type;
    
    if ((rv = getaddrinfo(node, port, &hints, &servinfo)) != 0){
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    for(p = servinfo; p != NULL; p = p->ai_next){
        if ((sock_clnt_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }

        if (connect(sock_clnt_fd, p->ai_addr, p->ai_addrlen) == -1){ 
            close(sock_clnt_fd); 
            perror("client: connect"); 
            continue; 
        }

        break;
    }

    if (p == NULL) { 
        fprintf(stderr, "client: failed to connect\n");
        exit(EXIT_FAILURE);
    }
    
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
   
    freeaddrinfo(servinfo);

    printf("client: connecting to %s\n", s);


    return sock_clnt_fd;
}