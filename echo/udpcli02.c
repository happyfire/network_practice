#include "netfoo.h"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr* pservaddr, socklen_t servlen);

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc!=2)
		err_quit("usage: udpcli <IPaddress>");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	dg_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
}

//相对于上一个版本 udpcli01.c，使用了udp connect

void dg_cli(FILE *fp, int sockfd, const struct sockaddr* pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE+1];
	struct sockaddr_in cliaddr;
	socklen_t len;

	Connect(sockfd, (struct sockaddr*)pservaddr, servlen);

	//调用connect后，可通过getsockname获得输出接口的ip地址和端口
	len = sizeof(cliaddr);
	getsockname(sockfd,(struct sockaddr*)&cliaddr, &len);
	char localAddr[INET_ADDRSTRLEN];
	Inet_ntop(AF_INET, &(cliaddr.sin_addr.s_addr), localAddr, INET_ADDRSTRLEN);

	printf("local address %s:%d\n", localAddr,ntohs(cliaddr.sin_port));


	while(Fgets(sendline, MAXLINE, fp)!=NULL){
		Write(sockfd, sendline, strlen(sendline));

		n = Read(sockfd, recvline, MAXLINE);

		recvline[n] = 0; //null terminate
		Fputs(recvline, stdout);
	}
}