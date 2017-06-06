CC = gcc

CFLAGS=-g -Wall
CFLAGS += -I../libnetfoo/lib
LDFLAGS += -L../libnetfoo/lib
LDFLAGS += -lnetfoo

all: server server2 client sumserver sumclient

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

clean:
	-rm *.o server server2 client sumserver sumclient
