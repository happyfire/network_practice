#include "netfoo.h"

void str_echo(int sockfd);

int main(int agrc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    
    for(;;){
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if( (childpid=fork())==0){
            //child process
            close(listenfd);  //close listening socket
            str_echo(connfd); //process the request
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

