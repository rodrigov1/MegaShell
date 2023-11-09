#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "../include/external_program.h"

#define BACKGROUND_NUM 2 // Working for two process in background at the same time max

pid_t fork_success;
int jobId = 0;
// sig_atomic_t child_exit_status;
int jobIdBG[BACKGROUND_NUM] = {0, 0}; // Job id of processes in background
int pidchldBG[BACKGROUND_NUM] = {0, 0}; // Pid of processes in background
int flags[BACKGROUND_NUM] = {0, 0}; // Flags for child process which ended

void chld_handler(int signal_number)
{
    /* Clean up the child process. */
    pid_t pid_ended; // Child's pid that just ended

    // pid_t waitpid(pid_t pid, int *status_ptr, int options); // Waitpid format
    /*
    -1: Esto significa que waitpid espera a cualquier proceso hijo. -1 es un comodín que indica que se debe esperar a cualquier proceso hijo que haya terminado.
    NULL: Aquí, no estamos interesados en recopilar ningún estado específico del proceso hijo, por lo que pasamos NULL.
    WNOHANG: Esta es una bandera que indica que waitpid no debe bloquearse si no hay procesos hijos que hayan terminado.
    En lugar de bloquearse, waitpid regresará inmediatamente con un valor de -1 si no hay procesos hijos disponibles para ser recogidos.
    */

    for (int i = 0; i < BACKGROUND_NUM; i++)
    {
        if ((pid_ended = waitpid(-1, NULL, WNOHANG)) == pidchldBG[i] && pidchldBG[i] != 0) // Comparing the pid (from a child that just ended) with the ones in background
        {
            flags[i] = 1;
            break;
        }
    }
}

char **clean_command(int argc, char *args[])
{
    int i = 0;
    while (i != argc)
    {
        if (strrchr(args[i], '&'))
        {
            args[i] = NULL;
        }
        i++;
    }
    return args;
}

void program_invocation(int argc, char *args[], int back)
{

    signal(SIGCHLD, chld_handler);

    char *cmd = args[0];                         // The first argument is the command
    char **cmd_args = clean_command(argc, args); // Command arguments

    args[argc] = NULL;

    if (argc > 0)
    {
        fork_success = fork();
        switch (fork_success)
        {
        case 0: // Child process

            // Check if args[0] is not null
            if (cmd != NULL)
            {

                // Execute the command in a shell
                execvp(cmd, cmd_args);

                // execvp only returns if there was an error
                fprintf(stderr, "Error executing the command in a shell\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "Null command\n");
                exit(EXIT_FAILURE);
            }

        case -1: // Error creating the child process
            fprintf(stderr, "Error creating the child process\n");
            exit(EXIT_FAILURE);

        default: // Parent process
            if (back == 0)
            {
                waitpid(fork_success, 0, 0);
            }
            else
            {
                jobId++;
                fprintf(stdout, "[%d] [%d] \n", jobId, fork_success);

                for (int i = 0; i < BACKGROUND_NUM; i++)
                { // Stores child's pid working in background
                    if (pidchldBG[i] == 0)
                    {
                        pidchldBG[i] = fork_success;
                        jobIdBG[i] = jobId;
                        break;
                    }
                }
            }
            for (int i = 0; i < BACKGROUND_NUM; i++)
            {
                if (flags[i] == 1)
                {
                    fprintf(stdout, "Done id: [%d] process: [%d]\n", jobIdBG[i], pidchldBG[i]);
                    flags[i] = 0;
                    pidchldBG[i] = 0;
                }
            }
            break;
        }
    }
}