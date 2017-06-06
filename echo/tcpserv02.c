#include "netfoo.h"

void str_echo(int sockfd);
void sig_chld(int signo);

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

    signal(SIGCHLD, sig_chld);
    
    for(;;){
        clilen = sizeof(cliaddr);
        if( (connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen))<0){
            if(errno == EINTR)
                continue; //back to fo
            else
                err_sys("accept error");
        }

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
    long arg1, arg2;
    ssize_t n;
    char line[MAXLINE];
    
    for(;;){
        if( (n=Readline(sockfd, line, MAXLINE))==0){
            return; // connection closed by other end
        }
        if(sscanf(line,"%ld%ld",&arg1,&arg2)==2){
            snprintf(line,sizeof(line),"%ld\n",arg1+arg2);
            
        }
        else{
            //snprintf(line,sizeof(line),"input error\n");
        }
        n = strlen(line);
        Writen(sockfd, line, n);
    }
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while( (pid=waitpid(-1, &stat, WNOHANG)) >0){
        printf("child %d terminated\n",pid);
    }
    return;
}
