#ifndef MLDR_CONNECTIONS
#define MLDR_CONNECTIONS

void *get_in_addr(struct sockaddr *sa);
int mldr_connect_to_client(int domain, int type, int flags, const char* port);

#endif /* MLDR_CONNECTIONS */