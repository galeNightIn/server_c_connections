#ifndef MLDR_CONNECT_SERVER
#define MLDR_CONNECT_SERVER

void *get_in_addr(struct sockaddr *sa);
int mldr_connect_to_server(int domain, int type, const char* node, const char* port);

#endif /* MLDR_CONNECT_SERVER */