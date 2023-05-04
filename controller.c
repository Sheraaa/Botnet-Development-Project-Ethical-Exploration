#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "header.h"
#include "utils_v2.h"

/**
 * PRE: servierIP : a valid IP address
 *      serverPort: a valid port number
 * POST: on success, connects a client socket to serverIP:serverPort ;
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int initSocketClient(char *serverIP, int serverPort);

/**
 *
 */
void executeCommands();

// LE LOUP (client)
int main(int argc, char **argv) {

  if (argc != 2) {
    perror("Must give a name or IP adress in argument\n");
  }

  int tabSockets[argc - 1];
  char tab_ip_address[argc - 1][18];
  int i = 1;
  int randomInt;

  while (i < argc) {
    hostname_to_ip(argv[1], tab_ip_address[i]);
    i++;
    randomInt = randomIntBetween(0, 9);
    tabSockets[i] = initSocketClient(tab_ip_address[i], TAB_PORTS[randomInt]);
  }

  fork_and_run1();
}

int initSocketClient(char *serverIP, int serverPort) {
  int sockfd = ssocket();
  sconnect(serverIP, serverPort, sockfd);
  return sockfd;
}
