// LE LOUP (client) + adresses IP
#include "header.h"
#include "utils_v2.h"

/**
 * PRE: servierIP : a valid IP address
 *      serverPort: a valid port number
 * POST: on success, connects a client socket to serverIP:serverPort ;
 *       -1 on failure.
 * RES: return socket file descriptor or -1
 */
int initSocketClient(char *serverIP, int serverPort);

/**
 * the child processus which listens on the given socket and display it on
 * screen.
 *
 * PRE: socketfd: a valid socketfd.
 * POST: display the response of the socket.
 */
void childReceiveCommand(void *socketfd, void *nbSockets);

/**
 * Closes all the sockets in tabSockets
 *
 * PRE: tabSockets: contains all the open sockets, nbSockets:
 * POST:
 **/
void disconnect(int *tabSockets, int nbSockets);

int main(int argc, char **argv) {

  if (argc < 2) {
    perror("Must give an IP adress !\n");
    exit(EXIT_FAILURE);
  }
  int randomInt, sockfd, i = 0, nbSockets = 0;
  int tabSockets[NB_SOCKETS];
  char *ip_address = argv[1];

  printf("Trying to connect to the server...\n");
  int port, ret;
  for (int i = 0; i < 10; i++) {
    sockfd = initSocketClient(ip_address, TAB_PORTS[i]);
    if (sockfd != -1) {
      tabSockets[nbSockets] = sockfd;
      port = TAB_PORTS[i];
      nbSockets++;
      printf("The controller is connected to the socket : %d \n", port);
    }
  }

  fork_and_run2(childReceiveCommand, tabSockets, &nbSockets);

  char buffer[SIZE_MESSAGE];
  int nbRd;
  char *msg = "\nCommande suivante -> ";
  swrite(1, msg, strlen(msg));

  while ((nbRd = sread(0, buffer, SIZE_MESSAGE)) != 0) {

    // check if user entered something
    if (buffer[0] != '\n') {
      for (int i = 0; i < nbSockets; i++) {
        nwrite(tabSockets[i], buffer, nbRd);
      }
    }
   // swrite(1, msg, strlen(msg));
  }

  disconnect(tabSockets, nbSockets);
}

// Processus child which reads the response of the zombie
void childReceiveCommand(void *tabSockets, void *nbSocketss) {
  char response[SIZE_MESSAGE];
  int nbSockets = *(int *)nbSocketss;
  int *sockets = (int *)tabSockets;
  int nbRd, ret;
  struct pollfd fds[NB_SOCKETS];

  // init poll
  for (int i = 0; i < nbSockets; i++) {
    fds[i].fd = sockets[i];
    fds[i].events = POLLIN;
  }

  // version sans boucle ou avec boucle while(1) et timeout à 1000??
  ret = poll(fds, nbSockets, -1);
  checkNeg(ret, "server poll error");

  // if (ret == 0) // none has answered yet
  //   continue;

  for (int i = 0; i < nbSockets; i++) {

    if (fds[i].revents & POLLIN) {

      while ((nbRd = sread(sockets[i], response, SIZE_MESSAGE)) != 0) {
        if (nbRd != 0) {
          nwrite(1, response, nbRd);
        }
      }
    }
  }
}

// establish a connection with the server
int initSocketClient(char *serverIP, int serverPort) {
  int sockfd = ssocket();
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(serverPort);
  inet_aton(serverIP, &addr.sin_addr);
  int ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
  if (ret == -1)
    return -1;
  return sockfd;
}

// closing all the open sockets
void disconnect(int *tabSockets, int nbSockets) {
  for (int i = 0; i < nbSockets; i++) {
    sclose(tabSockets[i]);
  }
}
