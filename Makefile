CFLAGS=-std=c11 -pedantic -Wall -g -Wvla -Werror -D_DEFAULT_SOURCE - D_XOPEN_SOURCE - D_BSD_SOURCE

all : zombie controller labo


zombie : zombie.o utils_v2.o server.o
	$(CC) $(CCFLAGS) -o $@ $^

zombie.o : zombie.c utils_v2.h common.h server.h
	$(CC) $(CCFLAGS) -c zombie.c

controller : controller.o utils_v2.o server.o
	$(CC) $(CCFLAGS) -o $@ $^

controller.o : controller.c utils_v2.h common.h server.h
	$(CC) $(CCFLAGS) -c controller.c

labo : labo.o utils_v2.o
	$(CC) $(CCFLAGS) -o $@ $^

labo.o : labo.c utils_v2.h
	$(CC) $(CCFLAGS) -c labo.c

utils_v2.o : utils_v2.c utils_v2.h 
	$(CC) $(CCFLAGS) -c utils_v2.c

server.o : server.c server.h common.h
	$(CC) $(CCFLAGS) -c server.c

clear : 
		clear

clean: 
	rm -f *.o
	rm -f zombie controller labo server
