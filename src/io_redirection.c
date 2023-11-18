#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/external_program.h"
/*
8. I/O redirection
*/
void io_redirection(int argc, char *args[])
{
    char *inputfile = NULL;
    char *outputfile = NULL;
    int in = 0;
    int out = 0;
    int flag = 0;
    char **cmd1 = (char **)malloc(sizeof(char *) * argc);
    int argc1 = 0;

    // Split the args in a program and his arguments, and the input and output files if they exist
    for (int i = 0; args[i] != NULL; i++)
    {
        if (flag == 0)
        {
            if (strcmp(args[i], "<") == 0)
            {
                in = 1;
                flag = 1;
            }
            else if (strcmp(args[i], ">") == 0)
            {
                out = 1;
                flag = 1;
            }
            else
            {
                cmd1[argc1] = strdup(args[i]);
                argc1++;
            }
        }
        else if (flag == 1)
        {
            if (in == 1 && out == 0)
            {
                inputfile = strdup(args[i]);
                flag = 0;
            }
            else if (out == 1)
            {
                outputfile = strdup(args[i]);
                flag = 0;
            }
        }
        else
        {
            flag = 1;
            break;
        }
    }
    cmd1[argc1] = NULL;

    pid_t pid;
    pid = fork();

    switch (pid)
    {
    case -1:
        perror("Error creating the child process\n");
        exit(EXIT_FAILURE);

    case 0:
        if (inputfile != NULL)
        {
            int fd_in = open(inputfile, O_RDONLY, 0777); // Open the file in read only mode with permissions 0777
            if (fd_in == -1)
            {
                perror("Redirection fail\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (outputfile != NULL)
        {
            int fd_out = open(outputfile, O_WRONLY | O_CREAT, 0777);
            if (fd_out == -1)
            {
                perror("Redirection fail\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        program_invocation(argc1, cmd1,0);
        exit(0); // This only executes if execvp fails
        break;

    default:
        waitpid(pid, 0, 0);
        break;
    }

    for (int i = 0; i < argc1; i++)
    {
        free(cmd1[i]);
    }

    free(cmd1);
    
    return;
}
