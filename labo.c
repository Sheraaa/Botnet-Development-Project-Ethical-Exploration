#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "header.h"
#include "utils_v2.h"

/**
* child Processus which execute the zombie programme
* 
*/
void execZombie();

int main(int argc, char ** argv){

    pid_t child_pid1 = fork_and_run0(execZombie);
    pid_t child_pid2 = fork_and_run0(execZombie);

    while (read(0, NULL, 1)!= 0 ){
      
    }

    kill(child_pid1, SIGTERM);
    kill(child_pid2, SIGTERM);
    
}


void execZombie(){
    execl("./zombie", "zombie", NULL); 
    exit(1);
}
