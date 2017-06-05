#include "netfoo.h"

void str_echo(int sockfd);

int main(int agrc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);
    
    for(;;){
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

        char cliaddr_p[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr_p, sizeof(cliaddr_p));
        printf("client %s:%d connected\n",cliaddr_p,ntohs(cliaddr.sin_port));

        if( (childpid=fork())==0){
            //child process
            close(listenfd);  //close listening socket
            str_echo(connfd); //process the request
            printf("client %s:%d closed\n",cliaddr_p,ntohs(cliaddr.sin_port));
            exit(0);
        }

        close(connfd); //parent closes connected socket
    }
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];
    
again:
    while( (n=read(sockfd, buf, MAXLINE)) >0 ){
        Writen(sockfd, buf, n);
    }
    
    if(n<0 && errno == EINTR)
        goto again;
    else if(n<0)
        err_sys("str_echo: read error");
}

