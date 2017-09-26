CC = gcc

CFLAGS :=-g -rdynamic -Wall -Werror -std=gnu99 -D_REENTRANT
LDFLAGS :=-lpthread

SOURCES = src/netcat.c src/utils.c
OBJECTS = $(SOURCES:.c=.o)

ROOTDIR=''
ifdef DESTDIR
	ROOTDIR = $(DESTDIR)
endif
INSTALLDIR = $(ROOTDIR)/usr/local/bin

all: netcat

netcat: src/main.o $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

install:
	@echo "Installing to " $(INSTALLDIR)
	[ -d $(INSTALLDIR) ] || mkdir -p $(INSTALLDIR)
	-cp -P netcat $(INSTALLDIR)

uninstall:
	@echo "Uninstalling from /usr/local/bin"
	-rm /usr/local/bin/netcat

clean:
	@echo "Cleanning project"
	-rm src/*.o src/*.d netcat
	@echo "Clean complete"

.PHONY:clean
.PHONY:install
.PHONY:uninstall

include src/main.d
include $(SOURCES:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$