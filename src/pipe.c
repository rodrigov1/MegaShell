#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../include/external_program.h"


void pipe_invocation(int argc, char *args[])
{
    char **cmd1 = (char **)malloc(sizeof(char *) * argc);
    char **cmd2 = (char **)malloc(sizeof(char *) * argc);
    int f = 0;
    int argc1 = 0;
    int argc2 = 0;

    // Splitting the command into two commands separated by the pipe
    for (int i = 0; i < argc; i++)
    {
        if (f == 0)
        {
            if (strcmp(args[i], "|") == 0)
            {
                f = 1;
            }
            else
            {
                cmd1[argc1] = strdup(args[i]);
                argc1++;
            }
        }
        else
        {
            cmd2[argc2] = strdup(args[i]);
            argc2++;
        }
    }

    cmd1[argc1] = NULL;
    cmd2[argc2] = NULL;

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Error creating the pipe\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        perror("Error creating the child process\n");
        exit(EXIT_FAILURE);
    case 0:
        close(fd[0]); // Close read end of the pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        program_invocation(argc1, cmd1, 0);
        exit(EXIT_FAILURE);
    default:
        close(fd[1]); // Close write end of the pipe
        pid_t pid2 = fork();
        switch (pid2) // Second fork
        {
        case -1:
            perror("Error creating the child process\n");
            exit(EXIT_FAILURE);
        case 0:
            close(fd[1]); // Close write end of the pipe
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            program_invocation(argc2, cmd2, 0);
            exit(EXIT_FAILURE);
        default:
            close(fd[0]); // Close read end of the pipe
            close(fd[1]); // Close write end of the pipe
            wait(NULL);
            break;
        }
        break;
    }
}
