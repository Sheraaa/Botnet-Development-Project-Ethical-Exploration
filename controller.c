#include "common.h"
#include "server.h"
#include "utils_v2.h"


/**
 * Groupe numéro 2:
 * @author Dorcas Kueze Nounga
 * @author Mariam Miclauri
 * @author Chehrazad Ouazzani
 */


/**
 * Child process which listens on the given socket and displays it on
 * screen.
 *
 * PRE: @param tabSockets: an array of valid socket file descriptors,
 *      @param nbSockets: the number of sockets.
 *
 * POST: Displays on screen the response of each socket file descriptor.
 *        Sends a SIGTERM signal to its parent when there is nothing to read on
 * sockets.
 */

void childReceiveCommand(void *tabSockets, void *nbSockets);

/**
 * Searches for all available ports on each IP address provided.
 *
 * PRE: @param tabSockets an empty array to be initialized with the size of
 *                   (NB_PORTS * argc - 1)
 *      @param nbSockets  a pointer to an integer variable that will be set to
 *                   the actual number of registered sockets
 *      @param argc       the number of command-line arguments
 *      @param argv       an array of strings containing the IP addresses to
 *scan
 *
 * POST: tabSockets is filled with the registered sockets
 *       nbSockets is set to the actual number of registered sockets.
 * RES:  display on screen the port number on which the controller is connected.
 **/

void getAllConnections(int *tabSockets, int *nbSockets, int argc, char **argv);

/**
 * Closes all the sockets in tabSockets.
 *
 * PRE: @param tabSockets: contains all the open sockets.
 *      @param nbSockets: the number of sockets.
 *
 * POST: closes all the sockets in tabSockets.
 **/
void disconnect(int *tabSockets, int nbSockets);

int main(int argc, char **argv) {
  char *msg;
  if (argc < 2) {
    msg = "Must give an IP adress !\n";
    swrite(1, msg, strlen(msg));
    exit(EXIT_FAILURE);
  }
  int randomInt, sockfd, i = 0, nbSockets = 0;
  int tabSockets[NB_PORTS * (argc - 1)];

  msg = "Trying to connect to the server...\n";
  swrite(1, msg, strlen(msg));
  getAllConnections(tabSockets, &nbSockets, argc - 1, argv + 1);

  pid_t childPID = fork_and_run2(childReceiveCommand, tabSockets, &nbSockets);

  char buffer[SIZE_MESSAGE];
  int nbRd;

  while ((nbRd = sread(0, buffer, SIZE_MESSAGE)) != 0) {

    // Checks if user entered something
    if (buffer[0] != '\n') {
      for (int i = 0; i < nbSockets; i++) {
        nwrite(tabSockets[i], buffer, nbRd);
      }
    }
  }
  skill(childPID, SIGTERM);
  disconnect(tabSockets, nbSockets);
  return 0;
}

// Child process which reads the response of the zombie's child.
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
    ret = spoll(fds, nbSockets, NB_SECONDS_POLL);

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

// Searches for all ports available on each IP adress given.
void getAllConnections(int *tabSockets, int *nbSockets, int argc, char **argv) {
  int port, sockfd;
  char msg[SIZE_MESSAGE];

  for (int i = 0; i < argc; i++) {
    for (int j = 0; j < NB_PORTS; j++) {
      sockfd = initSocketClient(argv[i], TAB_PORTS[j]);
      if (sockfd != -1) {
        tabSockets[*nbSockets] = sockfd;
        port = TAB_PORTS[j];
        (*nbSockets)++;
        sprintf(msg, "The controller is connected to the socket : %d \n", port);
        nwrite(1, msg, strlen(msg));
      }
    }
  }
}

// Closes all the open sockets.
void disconnect(int *tabSockets, int nbSockets) {
  for (int i = 0; i < nbSockets; i++) {
    sclose(tabSockets[i]);
  }
}
