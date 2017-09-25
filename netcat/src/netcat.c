#include "netcat.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"

int writen_n(int fd, const void *buf, int length)
{
    int written = 0;
    while(written < length){
        int nw = write(fd, (char*)buf + written, length - written);
        if(nw > 0){
            written += nw;
        }
        else if(nw == 0){
            break; //EOF
        }
        else if(errno != EINTR){
            perror("write");
            break;
        }
    }

    return written;
}

int create_tcp_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP);
    if(sockfd < 0){
        LOGE("socket error");
    }

    return sockfd;
}

void set_reuse_addr(int fd, bool on)
{
    int optval = on ? 1 : 0;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0){
        perror("set reuse addr");
    }
}

void listen_tcp_socket(int fd, int port)
{
    //set reuse addr
    set_reuse_addr(fd, true);

    //bind
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);
    if(bind(fd, (struct sockaddr *)&saddr, sizeof saddr) < 0){
        perror("bind error");
        abort();
    }

    //listen
    int ret = listen(fd, SOMAXCONN);
    if(ret < 0){
        perror("listen error");
        abort();
    }
}

int tcp_accept(int listenfd)
{
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof cli_addr;

    int connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if(connfd < 0){
        perror("accept failed");
        abort();
    }

    char cli_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, sizeof cli_ip);
    LOG("client %s:%d connected.\n", cli_ip, ntohs(cli_addr.sin_port));

    return connfd;
}

void tcp_connect(int fd, const char *host, int port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host, &server_addr.sin_addr);

    int r = connect(fd, (struct sockaddr *)&server_addr, sizeof server_addr);
    if(r < 0){
        perror("connect error");
        abort();
    }
}


