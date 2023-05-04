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

// LA MARIONNETTE (server)

/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port);


int main(int argc, char **argv) {

  int sockfd, newsockfd, i;
  StructMessage msg;
  int ret;
  struct pollfd fds[MAX_PLAYERS];

  int port;
  if (argc > 1) {
    port = argv[2]; //???
  } else {
    int randomInt = randomIntBetween(0, 9);
    port = TAB_PORTS[randomInt];
  }

  sockfd = initSocketServer(port);
  printf("Le serveur tourne sur le port : %i \n", port);
  int ret;

  while (1) {
    newsockfd = accept(sockfd, NULL, NULL);

    if (newsockfd > 0) {
      ret = sread(newsockfd, &msg, sizeof(msg));
      ret = swrite(newsockfd, &msg, sizeof(msg));
    }


  }
}

int initSocketServer(int port) {
  int sockfd = ssocket();
  int option = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
  sbind(port, sockfd);
  slisten(sockfd, BACKLOG);
  return sockfd;
}







