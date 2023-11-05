#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t fork_success;

void program_invocation(int argc, char *args[], int back)
{
    args[argc] = NULL;

    if (argc > 0)
    {
        fork_success = fork();
        switch (fork_success)
        {
        case 0: // Child process

            // Check if args[0] is not null
            if (args[0] != NULL)
            {
                char *cmd = args[0];    // The first argument is the command
                char **cmd_args = args; // Command arguments
                
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
            if (back != 0)
            {
                wait(NULL); // Wait for the child process if "back" is specified
            }
        }
    }
}
