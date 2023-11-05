#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../inlude/external_program.h"

/*
3. Program invocation
Las entradas del usuario que no sean comandos internos deben ser interpretados como la invocación de un programa. 
La misma tiene que ser realizada mediante fork y execl. Debe además soportar paths relativos y absolutos.
*/

pid_t fork_success;

void program_invocation(int argc, char *args[], int back){
    args[argc] = NULL;
    if (argc!=0){
        fork_success = fork();
        switch(fork_success)
        {
            case 0: // Child process
                execv("/usr/bin/bash",args); // Creating a terminal with those arguments
            
            case -1: // Error creating child process
                fprintf(stderr,"Error creating the program\n");
                exit (0);
            default:
                if (back != 0){
                    wait(NULL);
                }
                else{
                    exit(EXIT_SUCCESS);
                }
        }
    }
}