#ifndef _NETCAT_H
#define _NETCAT_H

#include <stdbool.h>

int writen_n(int fd, const void *buf, int length);

int create_tcp_socket();

void set_reuse_addr(int fd, bool on);

void listen_tcp_socket(int fd, int port);

int tcp_accept(int listenfd);

void tcp_connect(int fd, const char *host, int port);

void run(int tcpfd);

#endif //_NETCAT_H
