#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "netcat.h"
#include "utils.h"


static const char *short_options = "s:p:l";
static const struct option long_options[] = {
        {"host", required_argument, NULL, 's'},
        {"port", required_argument, NULL, 'p'},
        {"listen", no_argument, NULL, 'l'},
        {NULL, 0, NULL, 0}
};

int main(int argc, char *argv[])
{
    char *host = NULL;
    int port = 0;
    int listen = 0;

    int opt = 0;
    while( (opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1){
        switch (opt){
            case '?':
                fprintf(stdout, "Usage: %s -s <host> -p <port> [-l]\n", argv[0]);
                return 0;
            case 's':
                host = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'l':
                listen = 1;
                break;
        }
    }

    if(port <= 0){
        LOGE("port is wrong: %d",port);
    }

    int fd = create_tcp_socket();
    if(listen){
        LOG("server running.");
        listen_tcp_socket(fd, port);
        int connfd = tcp_accept(fd);
        LOG("connfd=%d",connfd);

        //TODO: run thread
        close(connfd);
    }
    else{
        LOG("client running.");
        tcp_connect(fd, host, port);

    }

    return 0;
}