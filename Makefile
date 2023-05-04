CC=gcc
CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

all: zombie controller #labo

zombie : zombie.o utils_v2.o
	$(CC) $(CCFLAGS) -o zombie zombie.o utils_v2.o

zombie.o: zombie.c utils_v2.h header.h
	$(CC) $(CCFLAGS) -c zombie.c

controller : controller.o utils_v2.o
	$(CC) $(CCFLAGS) -o controller controller.o utils_v2.o

controller.o: controller.c utils_v2.h header.h
	$(CC) $(CCFLAGS) -c controller.c

# labo : labo.o utils_v2.o
# 	$(CC) $(CCFLAGS) -o labo labo.o utils_v2.o
#
# labo.o: labo.c utils_v2.h messages.h
# 	$(CC) $(CCFLAGS) -c labo.c

utils_v2.o: utils_v2.c utils_v2.h
	$(CC) $(CCFLAGS) -c utils_v2.c

clear :
		clear

clean :
	rm -f *.o
	rm -f zombie
	rm -f controller
#	rm -f labo

dev: clear all
	./createInscriptionRequests
	./pgmInscription < DemandesInscriptions.bin
