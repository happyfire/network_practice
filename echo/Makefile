CC = gcc

CFLAGS=-g -Wall
CFLAGS += -I../libnetfoo/lib
LDFLAGS += -L../libnetfoo/lib
LDFLAGS += -lnetfoo

all: server server2 client sumserver sumclient client_select client_select2 server_select server_poll

server: tcpserv01.o
	$(CC) $(LDFLAGS) -o server tcpserv01.o

server2: tcpserv02.o
	$(CC) $(LDFLAGS) -o server2 tcpserv02.o

client: tcpcli01.o
	$(CC) $(LDFLAGS) -o client tcpcli01.o

sumserver: sumserver.o
	$(CC) $(LDFLAGS) -o sumserver sumserver.o

sumclient: sumclient.o
	$(CC) $(LDFLAGS) -o sumclient sumclient.o

client_select: tcpcli_select.o
	$(CC) $(LDFLAGS) -o client_select tcpcli_select.o

client_select2: tcpcli_select02.o
	$(CC) $(LDFLAGS) -o client_select2 tcpcli_select02.o

server_select: tcpserv_select.o
	$(CC) $(LDFLAGS) -o server_select tcpserv_select.o

server_poll: tcpserv_poll.o
	$(CC) $(LDFLAGS) -o server_poll tcpserv_poll.o

clean:
	-rm *.o server server2 client sumserver sumclient
