// LE LOUP (client) + noms domaines ou adresses IP
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
 * the child processus who send the command on the given socket.
 *
 * PRE: command : a valid command, socketfd: a valid socketfd
 * POST:
 *
 */
void childSendCommand(void *buffer, void *socketfd, void *sizeRead);

/**
 * the child processus who listen on the given socket and display it on screen.
 *
 * PRE: socketfd: a valid socketfd.
 * POST: display the response of the socket.
 */
void childReceiveCommand(void *socketfd);

int main(int argc, char **argv) {

  if (argc <= 2) {
    perror("Must give 1: valid port & 2: a name or IP adress !\n");
    exit(EXIT_FAILURE);
  }

  // int tabSockets[argc - 1];
  // char tab_ip_address[argc - 1][18];
  // int i = 1;
  // int randomInt;
  //
  // while (i < argc) {
  //   i++;
  // randomInt = randomIntBetween(0, 9);
  //   tabSockets[i] = initSocketClient(tab_ip_address[i],
  //   TAB_PORTS[randomInt]);
  // }
  printf("Le controller se connecte avec le zombie...\n");
  char tab_ip_address[18];
  int port = atoi(argv[1]);
  hostname_to_ip(argv[2], tab_ip_address);
  int sockfd = initSocketClient(tab_ip_address, port);
  printf("Le controller est connecté sur le socket : %d \n", sockfd);

  char buffer[SIZE_MESSAGE];
  int nbRd;
  pid_t childSendPID, childReceivePID;
  char *msg = "\nCommande suivante: \n  -> ";
  while ((nbRd = sread(0, buffer, SIZE_MESSAGE)) != 0) {
    if (buffer[0] != '\n') {
      buffer[nbRd - 1] = '\0';

      childSendPID = fork_and_run3(childSendCommand, buffer, &sockfd, &nbRd);
      swaitpid(childSendPID, NULL, 0);

      childReceivePID = fork_and_run1(childReceiveCommand, &sockfd);
      swaitpid(childReceivePID, NULL, 0);

      swrite(1, msg, strlen(msg));
    }
  }
}

// Processus child who sents the command to a zombie
void childSendCommand(void *buffer, void *socketfd, void *sizeRead) {
  char *script = buffer;
  int sockfd = *(int *)socketfd;
  int nbRd = *(int *)sizeRead;
  swrite(sockfd, script, nbRd);
}

// Processus child who read the response of the zombie
void childReceiveCommand(void *socketfd) {
  char response[SIZE_MESSAGE];
  int sockfd = *(int *)socketfd;
  int nbRd = sread(sockfd, response, SIZE_MESSAGE);
  swrite(1, response, nbRd);
}

// establish a connection with the server
int initSocketClient(char *serverIP, int serverPort) {
  int sockfd = ssocket();
  sconnect(serverIP, serverPort, sockfd);
  return sockfd;
}
