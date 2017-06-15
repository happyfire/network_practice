#include "netfoo.h"

int main(int argc, char ** argv)
{
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char mesg[MAXLINE];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	void sig_chld(int);
	void str_echo(int sockfd);

	// create listening TCP socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	// create udp socket
	udpfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	signal(SIGCHLD, sig_chld);//must call waitpid()

	FD_ZERO(&rset);
	maxfdp1 = max(listenfd, udpfd)+1;
	for(;;){
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		if(( nready = select(maxfdp1, &rset, NULL, NULL, NULL)) <0 ) {
			if(errno == EINTR)
				continue; //back to for()
			else
				err_sys("select error");
		}

		if(FD_ISSET(listenfd, &rset)){
			len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			if((childpid = Fork())==0){
				//child process
				Close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			Close(connfd); //parent close connected socket
		}

		if(FD_ISSET(udpfd, &rset)){
			len = sizeof(cliaddr);
			n = Recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr*)&cliaddr, &len);
			Sendto(udpfd, mesg, n, 0, (struct sockaddr*)&cliaddr, len);
		}
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