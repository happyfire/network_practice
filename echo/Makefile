CC = gcc

CFLAGS=-g -Wall
CFLAGS += -I../libnetfoo/lib
LDFLAGS += -L../libnetfoo/lib
LDFLAGS += -lnetfoo

server: tcpserv01.o
	$(CC) $(LDFLAGS) -o server tcpserv01.o

clean:
	-rm *.o server