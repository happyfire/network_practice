#ifndef __netfoo_h
#define __netfoo_h

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */

#include 	<errno.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include	<unistd.h> //fork

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Define some port number that can be used for our examples */
#define	SERV_PORT		 9877			/* TCP and UDP */
#define	SERV_PORT_STR	"9877"			/* TCP and UDP */
#define	UNIXSTR_PATH	"/tmp/unix.str"	/* Unix domain stream */
#define	UNIXDG_PATH		"/tmp/unix.dg"	/* Unix domain datagram */

#endif /* __netfoo_h */

ssize_t	writen(int fd, const void *vptr, size_t n);
void	 Writen(int, void *, size_t);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);