#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
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
    int continuar;
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

    if (in == 1)
    {
        int fd1;
        fd1 = open(inputfile, O_RDONLY);
        if (fd1 == -1)
        {
            perror("Error opening the file\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (dup2(fd1, STDIN_FILENO) == -1) // fd is the file descriptor of the file we want to redirect to STDIN_FILENO
            {
                perror("Error duplicating the file descriptor\n");
                exit(EXIT_FAILURE);
            }
            pid_t pid = fork();
            switch (pid)
            {
            case -1:
                perror("Error creating the child process\n");
                exit(EXIT_FAILURE);
            case 0:
                execvp(cmd1[0], cmd1);
            default:
                wait(NULL);
                close(fd1); // Closing the file descriptor
                continuar = 1;
                break;
            }
        }
    } else{
        continuar = 1;
    }

    if (continuar && out == 1)
    {
        int fd2;
        fd2 = open(outputfile, O_CREAT | O_WRONLY, 0777); // 0777 is the permission of the file created
        if (fd2 == -1)
        {
            perror("Error opening the file\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (dup2(fd2, STDOUT_FILENO) == -1) // fd is the file descriptor of the file we want to redirect to STDOUT_FILENO
            {
                perror("Error duplicating the file descriptor\n");
                exit(EXIT_FAILURE);
            }
            pid_t pid2 = fork();
            switch (pid2)
            {
            case -1:
                perror("Error creating the child process\n");
                exit(EXIT_FAILURE);
            case 0:
                execvp(cmd1[0], cmd1);
            default:
                waitpid(pid2, NULL, 0);
                close(fd2); // Closing the file descriptor
                break;
            }
        }
    }

    for (int i = 0; i < argc1; i++)
    {
        free(cmd1[i]);
    }
    free(cmd1);
    return;
}
