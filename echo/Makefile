CC = gcc

CFLAGS=-g -Wall
CFLAGS += -I../libnetfoo/lib
LDFLAGS += -L../libnetfoo/lib
LDFLAGS += -lnetfoo

all: server client

server: tcpserv01.o
	$(CC) $(LDFLAGS) -o server tcpserv01.o

client: tcpcli01.o
	$(CC) $(LDFLAGS) -o client tcpcli01.o

clean:
	-rm *.o server client