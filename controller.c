#include "common.h"
#include "utils_v2.h"
#include "server.h"

/**
 * the child processus which listens on the given socket and display it on
 * screen.
 *
 * PRE: socketfd: a valid socketfd.
 * POST: display the response of the socket.
 */
void childReceiveCommand(void *socketfd, void *nbSockets);

/**
 * Search for all ports available on each IP adress given
 * PRE: tabSockets: an empty array ready to be initialized with
 *      the size of (NB_PORTS * argc - 1) ,
 *      nbSockets: an empty variable, argc: the number of arguments,
 *      argv: the IP adresses given by the user.
 * POST: tabSockets: filled with the registered sockets.
 *        nbSockets: initialized with the actual number of registered sockets.
 * RES: display on screen which port the controller is connected.
 **/
void getAllConnections(int *tabSockets, int *nbSockets, int argc, char **argv);

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
  int tabSockets[NB_PORTS * (argc - 1)];

  printf("Trying to connect to the server...\n");
  getAllConnections(tabSockets, &nbSockets, argc - 1, argv + 1);

  pid_t childPID = fork_and_run2(childReceiveCommand, tabSockets, &nbSockets);

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
  skill(childPID, SIGTERM);
  disconnect(tabSockets, nbSockets);
  return 0;
}

// Processus child which reads the response of the zombie
void childReceiveCommand(void *tabSockets, void *nbSocketss) {
  char response[SIZE_MESSAGE];
  int nbSockets = *(int *)nbSocketss;
  int *sockets = (int *)tabSockets;
  int nbRd, ret;
  struct pollfd fds[NB_PORTS];
  int realNbSockets = nbSockets;

  // init poll
  for (int i = 0; i < nbSockets; i++) {
    fds[i].fd = sockets[i];
    fds[i].events = POLLIN;
  }

  while (realNbSockets > 0) {
    ret = spoll(fds, nbSockets, 1000);

    if (ret == 0)
      continue;

    for (int i = 0; i < nbSockets; i++) {
      if (fds[i].revents & POLLIN) {

        nbRd = sread(sockets[i], response, SIZE_MESSAGE);
        if (nbRd > 0) {
          nwrite(1, response, nbRd);
        } else if (nbRd == 0) {
          realNbSockets--;
        }
      }
    }
  }
  skill(getppid(), SIGTERM);
}

// Search for all ports available on each IP adress given
void getAllConnections(int *tabSockets, int *nbSockets, int argc, char **argv) {
  int port, sockfd;

  for (int i = 0; i < argc; i++) {
    for (int j = 0; j < NB_PORTS; j++) {
      sockfd = initSocketClient(argv[i], TAB_PORTS[j]);
      if (sockfd != -1) {
        tabSockets[*nbSockets] = sockfd;
        port = TAB_PORTS[j];
        (*nbSockets)++;
        printf("The controller is connected to the socket : %d \n", port);
      }
    }
  }
}

// closing all the open sockets
void disconnect(int *tabSockets, int nbSockets) {
  for (int i = 0; i < nbSockets; i++) {
    sclose(tabSockets[i]);
  }
}
