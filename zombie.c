// LA MARIONNETTE (server) + port
#include "header.h"
#include "utils_v2.h"

int tabSockets[MAX_CONNECTION];


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
  char msg[SIZE_MESSAGE];
  int ret;
  StructPort structPort;
  struct pollfd fds[MAX_CONNECTION];

  if (argc > 1) {
    int port = atoi(argv[1]);
    sockfd = initSocketServer(port);
    printf("The server listens on the port : %d \n", port);
  } else {
    int randomInt = randomIntBetween(0, 9);
    if (!structPort.isUsed) {
      structPort.port = TAB_PORTS[randomInt];
      structPort.isUsed = true;
      sockfd = initSocketServer(structPort.port);
      printf("The server listens on the port : %d \n", structPort.port);
    }
  }

  while (1) {
    // newsockfd = accept(sockfd, NULL, NULL);
    newsockfd = saccept(sockfd); // quand labo mettre accept!!

    if (newsockfd > 0) {
      while (sread(newsockfd, msg, sizeof(msg)) != 0) {
        fork_and_run2(childExec, &newsockfd, msg);
      }
      sclose(newsockfd);
      printf("Connexion finie\n");
    }
  }
  sclose(sockfd);
}

// child process run the command
void childExec(void *sockfd, void *command) {
  char *script = command;
  int newsockfd = *(int *)sockfd;

  // redirection STDIN, STDERR, STDOUT
  dup2(newsockfd, STDIN_FILENO);
  dup2(newsockfd, STDOUT_FILENO);
  dup2(newsockfd, STDERR_FILENO);

  execl("/bin/sh", "programme_inoffensif", "-c", script, NULL);
  // system(script);
  perror("Something went wrong with execl\n");
  exit(EXIT_FAILURE);
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
