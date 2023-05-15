#include "common.h"
#include "utils_v2.h"

/**
 * Child process which launches the zombie program with the given port.
 *
 * PRE: @param port: a valid port.
 * POST: launches a zombie the given port.
 */
void execZombie(void *port);

int main(int argc, char **argv) {
  int randomInt1 = randomIntBetween(0, 9);
  int port1 = TAB_PORTS[randomInt1];
  int randomInt2;

  do {
    randomInt2 = randomIntBetween(0, 9);
  } while (randomInt2 == randomInt1);

  int port2 = TAB_PORTS[randomInt2];

  pid_t childPID1, childPID2;

  childPID1 = fork_and_run1(execZombie, &port1);
  childPID2 = fork_and_run1(execZombie, &port2);

  while (sread(0, NULL, 1) != 0) {
  }

  skill(childPID1, SIGTERM);
  skill(childPID2, SIGTERM);
}

// Child process which runs a zombie.
void execZombie(void *the_port) {
  int port = *(int *)the_port;
  char port_str[SIZE_MESSAGE];
  sprintf(port_str, "%d", port);
  sexecl("./zombie", "./zombie", port_str, NULL);
}
