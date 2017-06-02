MYLIB = libnetfoo.a
MYINC = netfoo.h

all: build install clean

build: $(MYLIB)

cc = gcc

INSTDIR = ../lib

INCLUDE = .

CFLAGS=-g -Wall



$(MYLIB): $(MYLIB)(error.o) $(MYLIB)(writen.o)

error.o: $(MYINC)

clean:
	-rm *.o $(MYLIB)

install: $(MYLIB)
	@if [ -d $(INSTDIR) ];\
	then \
		cp $(MYLIB) $(INSTDIR);\
		cp $(MYINC) $(INSTDIR);\
		echo "Installed in $(INSTDIR)";\
	else\
		echo "Sorry, $(INSTDIR) does not exist";\
	fi

