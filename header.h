#ifndef _MESSAGESH
#define _MESSAGESH

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <stdbool.h>

#define SERVER_PORT 9502
#define SERVER_IP "127.0.0.1" /* localhost */
#define BACKLOG 5
#define SIZE_MESSAGE 255
#define MAX_CONNECTION 2


static const int TAB_PORTS[10] = {1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034};

#endif
