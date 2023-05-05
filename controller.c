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
void childSendCommand(void *command, void *socketfd);

/**
 * the child processus who listen on the given socket and display it on screen.
 *
 * PRE: socketfd: a valid socketfd.
 * POST: display the response of the socket.
 */
void childReceiveCommand(void *socketfd);

int main(int argc, char **argv) {

  if (argc != 2) {
    perror("Must give a name or IP adress in argument \n");
    exit(EXIT_FAILURE);
  }

  // int tabSockets[argc - 1];
  // char tab_ip_address[argc - 1][18];
  // int i = 1;
  int randomInt;
  //
  // while (i < argc) {
  //   i++;
  randomInt = randomIntBetween(0, 9);
  //   tabSockets[i] = initSocketClient(tab_ip_address[i],
  //   TAB_PORTS[randomInt]);
  // }
  printf("Le controller se connecte avec le zombie...\n");
  char tab_ip_address[18];
  hostname_to_ip(argv[1], tab_ip_address);
  int sockfd = initSocketClient(tab_ip_address, 1026);
  printf("Le controller est connecté \n");

  char buffer[SIZE_MESSAGE];
  char response[SIZE_MESSAGE];
  int nbRd;
  char *msg = "\nCommande suivante: \n  - ";
  // printf("Entrez votre commande: ");
  while ((nbRd = sread(0, buffer, SIZE_MESSAGE)) != 0) {
    buffer[nbRd - 1] = '\0';
    swrite(sockfd, buffer, nbRd); // rajouter +1 ??
    nbRd = sread(sockfd, response, sizeof(response));
    swrite(1, response, nbRd);
    swrite(1, msg, strlen(msg));
  }
}

int initSocketClient(char *serverIP, int serverPort) {
  int sockfd = ssocket();
  sconnect(serverIP, serverPort, sockfd);
  return sockfd;
}
