#include "netfoo.h"

void str_cli(FILE* fp, int sockfd);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    
    if(argc!=2)
        err_quit("usage: tcpcli <IPaddress>");
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    signal(SIGPIPE, SIG_IGN);
    
    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    str_cli(stdin, sockfd);
    exit(0);
}

void str_cli(FILE* fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    
    while (Fgets(sendline, MAXLINE, fp)!=NULL) {

        Writen(sockfd, sendline, strlen(sendline));
        
        if(Readline(sockfd, recvline, MAXLINE) == 0 ){
            err_quit("str_cli: server terminated prematurely");
        }
        
        Fputs(recvline, stdout);
    }
}
