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
#include <pthread.h>

#include "mldr_send_recv.h"
#include "mldr_connect_client.h"

#define BACKLOG 1
#define MAXDATASIZE 100

int fd[2];	

static void* send_thread_function(void* arg){
    int sock_fd,
        result,
        len;
    
    char cmd[MAXDATASIZE] = " ";
    sock_fd = *((int *)arg);
    
    while(strcmp(cmd, "exit") != 0){
        result = read(fd[0], cmd, MAXDATASIZE);
        len = strlen(cmd);
        printf("SEND_THREAD:\nread %d bytes\nstring size: %d\n", result, len);
        printf("string: %s\n", cmd);
        if(len == 0) break;
        mldr_send(sock_fd, cmd, len, 0);
        printf("SERVER: sent %s message\n\n", cmd);
    }

    close(fd[0]);
    close(sock_fd);
}

static void* receive_thread_function(void* arg){
    int sock_fd,
        result,
        len;
    
    char cmd[MAXDATASIZE]= " ";
    sock_fd = *((int *)arg);
    int numbytes = 0;	

    while(strcmp(cmd, "exit") != 0){
        mldr_recv(sock_fd, cmd, MAXDATASIZE, 0, &numbytes);
        printf("RECEIVE_THREAD:\n");
        printf("SERVER:recived %s message\nnumber of bytes:%d\n", cmd, numbytes);
        len = strlen(cmd);
        write(fd[1], cmd, len + 1);
        printf("wrote %d bytes\nstring: %s in pipe\n\n", len + 1, cmd);
        if(numbytes == 0) break;
    }
    
    close(fd[1]);
    close(sock_fd);
}

int main(int argc, char const *argv[])
{
    const char* SEND_PORT = "5000";
    const char* RECEIVE_PORT = "4500";

    int     srv_send_fd = 0,
            srv_receive_fd = 0, 
            conn_send_sock = 0,
            conn_receive_sock = 0,
            numbytes = 0,
            result_pipe = 0;

    pthread_t     recv_thread, send_thread;
    
    struct sockaddr_storage     clnt_addr, clnt_addr_port;
    socklen_t     addrlen_send, addrlen_recv; 

    char s[INET6_ADDRSTRLEN] = " ";
    char s_port[INET6_ADDRSTRLEN] = " ";

    srv_send_fd = mldr_connect_to_client(PF_INET, SOCK_STREAM, AI_PASSIVE, SEND_PORT, BACKLOG);
    srv_receive_fd = mldr_connect_to_client(PF_INET, SOCK_STREAM, AI_PASSIVE, RECEIVE_PORT, BACKLOG);
    printf("server: waiting for connections...\n");
    while(1){
        addrlen_send = sizeof clnt_addr; 
        addrlen_recv = sizeof clnt_addr_port;

        conn_send_sock = accept(srv_send_fd, (struct sockaddr *)&clnt_addr, &addrlen_send);
        conn_receive_sock = accept(srv_receive_fd, (struct sockaddr *)&clnt_addr_port, &addrlen_recv);

        if (conn_send_sock  == -1) { 
            perror("Accept"); 
            continue; 
        }

        if (conn_receive_sock  == -1) { 
            perror("Accept"); 
            continue; 
        }


        inet_ntop(clnt_addr.ss_family, get_in_addr((struct sockaddr *)&clnt_addr), s, sizeof s);
        inet_ntop(clnt_addr_port.ss_family, get_in_addr((struct sockaddr *)&clnt_addr_port), s_port, sizeof s_port);
        printf("server: got connection from %s\n", s);	
        printf("server: got connection from %s\n", s_port);

        if((result_pipe = pipe(fd)) < 0) perror("pipe ");
        pthread_create(&recv_thread, NULL, send_thread_function, &conn_send_sock);
        pthread_create(&send_thread, NULL, receive_thread_function, &conn_receive_sock);

        pthread_join(recv_thread, NULL); 
        printf("RECV Thread ended\n");
        close(conn_receive_sock);   
        
        pthread_join(send_thread, NULL);
        printf("SEND Thread ended\n");
        close(conn_send_sock);
        
    }
        close(srv_receive_fd);
        close(srv_send_fd);

    return 0;
}