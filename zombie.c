// LA MARIONNETTE (server) + port
#include "header.h"
#include "utils_v2.h"
// //#define TAB_PORTS [10] = {1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1034}

/**
 * PRE:  port: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port);

/**
 * PRE:  port: a valid port number, sockfd: a valid socket file descriptor,
 * address_ip a valid IP address, command: the command on shell bash
 *
 * POST: execute the command bash and return the answer of the command
 */
void childExec(void *sockfd, void *command);

int main(int argc, char **argv) {

  int sockfd, newsockfd, i;
  char *msg;
  int ret;
  // struct pollfd fds[MAX_PLAYERS];

  char *port;
  if (argc > 1) {
    strcpy(port, argv[2]);
  } else {
    int randomInt = randomIntBetween(0, 9);
 //   strcpy(port, TAB_PORTS[randomInt]);
  }

  sockfd = initSocketServer(SERVER_PORT);
  printf("Le serveur tourne sur le port : %s \n", port);
  pid_t childPID;

  while (1) {
    // newsockfd = accept(sockfd, NULL, NULL);
    newsockfd = saccept(sockfd); // quand labo mettre accept!!
    swrite(1, "Le controller est connecté sur le zombie...\n", 60);

    if (newsockfd > 0) {
      ret = sread(newsockfd, msg, strlen(msg) + 1); // +1 ????
      childPID = fork_and_run2(childExec, &newsockfd, msg);
      swaitpid(childPID, NULL, 0);
      // ret = swrite(newsockfd, &msg, sizeof(msg));
    }
  }
  sclose(newsockfd);
  sclose(sockfd);
}

// établir une connexion du sock passif
int initSocketServer(int port) {
  int sockfd = ssocket();
  int option = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));
  sbind(port, sockfd);
  slisten(sockfd, BACKLOG);
  return sockfd;
}

void childExec(void *sockfd, void *command) {
  char *script = command;
  int newsockfd = *(int *)sockfd;

  // redirection STDIN, STDERR, STDOUT
  dup2(newsockfd, STDIN_FILENO);
  dup2(newsockfd, STDOUT_FILENO);
  dup2(newsockfd, STDERR_FILENO);

  char *args[] = {"/bin/bash", "-c", command, NULL};
  sexecl(sockfd, command);
  perror("Something went wrong with execl\n");
  exit(EXIT_FAILURE);
}
