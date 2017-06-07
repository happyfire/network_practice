#include "netfoo.h"

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	fd_set rset, allset;
	char buf[MAXLINE];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;
	maxi = -1; //index into client[] array
	for(i=0; i<FD_SETSIZE; i++){
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for(;;){
		rset = allset; //structure assignment
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(listenfd, &rset)){
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

			char cliaddr_p[INET_ADDRSTRLEN];
        	inet_ntop(AF_INET, &cliaddr.sin_addr, cliaddr_p, sizeof(cliaddr_p));
        	printf("client %s:%d connected\n",cliaddr_p,ntohs(cliaddr.sin_port));

			for(i=0; i<FD_SETSIZE; i++){
				if(client[i]<0){
					client[i] = connfd; //save fd
					break;
				}
			}

			if(i==FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset); //add new fd to set
			if(connfd > maxfd)
				maxfd = connfd;
			if(i>maxi)
				maxi = i;
			if(--nready<=0)
				continue; //no more readable fd
		}

		for(i=0; i<=maxi; i++){
			if( (sockfd=client[i])<0)
				continue;
			if(FD_ISSET(sockfd, &rset)){
				if((n=Read(sockfd, buf, MAXLINE))==0){
					//connection closed by client
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i]=-1;

					printf("client %d fd=%d closed\n",i, sockfd);
				}
				else{
					Writen(sockfd, buf, n);
				}

				if(--nready<=0){
					break; //no more readable fds
				}
			}
		}

	}
}

