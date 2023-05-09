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
 * the child processus who listen on the given socket and display it on screen.
 *
 * PRE: socketfd: a valid socketfd.
 * POST: display the response of the socket.
 */
void childReceiveCommand(void *socketfd);

int main(int argc, char **argv) {

  if (argc < 2) {
    perror("Must give an IP adress !\n");
    exit(EXIT_FAILURE);
  }

  int randomInt, sockfd, i = 0, nbSockets = 0;
  int tabSockets[10];
  char ip_address[18];
  hostname_to_ip(argv[1], ip_address);

  printf("Trying to connect to the server...\n");
  int port;
  for (int i = 0; i < 10; i++) {
    sockfd = initSocketClient(ip_address, TAB_PORTS[i]);
    if (sockfd > 0) {
      tabSockets[nbSockets] = sockfd;
      port = TAB_PORTS[i];
      nbSockets++;
 //     printf("%d ; ", port);
    }
  }

  printf("Le controller est connecté sur le socket : %d \n", port);

  char buffer[SIZE_MESSAGE];
  int nbRd;
  pid_t childSendPID, childReceivePID;
  char *msg = "\nCommande suivante: \n  -> ";

  while ((nbRd = sread(0, buffer, SIZE_MESSAGE)) != 0) {
    // check if user entered something
    if (buffer[0] != '\n') {
      buffer[nbRd - 1] = '\0';
      swrite(sockfd, buffer, nbRd);
    }
  }
  childReceivePID = fork_and_run1(childReceiveCommand, &sockfd);
  swrite(1, msg, strlen(msg));

  // closing all the open sockets
  for (int i = 0; i < nbSockets; i++) {
    close(tabSockets[i]);
  }
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
  // sconnect(serverIP, serverPort, sockfd);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr)); // en System V /
  addr.sin_family = AF_INET;
  addr.sin_port = htons(serverPort);
  inet_aton(serverIP, &addr.sin_addr);
  int ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
  return sockfd;
}
