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
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];
    
    FD_ZERO(&rset);
    for(;;){
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd)+1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        
        if(FD_ISSET(sockfd, &rset)){
            //socket is readable
            if(Readline(sockfd, recvline, MAXLINE)==0){
                err_quit("str_cli: server terminated prematurely");
            }
            Fputs(recvline, stdout);
        }
        
        if(FD_ISSET(fileno(fp), &rset)){
            //input is readable
            if(Fgets(sendline, MAXLINE, fp)==NULL){
                return; //all done
            }
            Writen(sockfd, sendline, strlen(sendline));
        }
    }
    
//    while (Fgets(sendline, MAXLINE, fp)!=NULL) {
//
//        Writen(sockfd, sendline, strlen(sendline));
//        
//        if(Readline(sockfd, recvline, MAXLINE) == 0 ){
//            err_quit("str_cli: server terminated prematurely");
//        }
//        
//        Fputs(recvline, stdout);
//    }
}
