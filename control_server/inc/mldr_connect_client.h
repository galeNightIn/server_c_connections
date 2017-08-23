#ifndef MLDR_CONNECT_CLIENT
#define MLDR_CONNECT_CLIENT

void *get_in_addr(struct sockaddr *sa);
int mldr_connect_to_client(int domain, int type, int flags, const char* port, const int backlog);


#endif /* MLDR_CONNECT_CLIENT */