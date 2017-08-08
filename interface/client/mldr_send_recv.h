#ifndef MLDR_SEND_RECV
#define MLDR_SEND_RECV

void mldr_send(int sockfd, const char *buf, size_t len, int flags);
void mldr_recv(int sockfd, char *buf, size_t len, int flags, int *numbytes);

#endif /* MLDR_SEND_RECV */