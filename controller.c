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
 * the child processus who execute the command given in argument.
 *
 */
void executeCommands();

int main(int argc, char **argv) {

  if (argc != 2) {
    perror("Must give a name or IP adress in argument\n");
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
  printf("Le controller se connecte avec le zombie\n");
  char tab_ip_address[18];
  hostname_to_ip(argv[1], tab_ip_address);
  int sockfd = initSocketClient(tab_ip_address, SERVER_PORT);

  char buffer[SIZE_MESSAGE];
  int nbRd;
  while ((nbRd = sread(1, buffer, SIZE_MESSAGE)) != 0) {
    buffer[nbRd - 1] = '\0';

    swrite(sockfd, buffer, strlen(buffer)); // rajouter +1 ??
  }
}

int initSocketClient(char *serverIP, int serverPort) {
  int sockfd = ssocket();
  sconnect(serverIP, serverPort, sockfd);
  return sockfd;
}













