#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "../include/manage_commands.h"
#include "../include/external_program.h"
#include "../include/cd.h"
#include "../include/clr.h"
#include "../include/quit.h"
#include "../include/echo.h"
#include "../include/pipe.h"

#define RESET_COLOR	"\x1b[0m"
#define GREEN		"\x1b[32m"


char* get_Command(void);
void execute_Command(char* command);

char buffer[PATH_MAX];
char hostname[HOST_NAME_MAX];
char username[LOGIN_NAME_MAX];

void prompt()
{
    if (gethostname(hostname, HOST_NAME_MAX) == -1)
    {
        fprintf(stderr, "Error al obtener el nombre del host");
        exit(1);
    }

    if (getlogin_r(username, LOGIN_NAME_MAX) == -1)
    {
        fprintf(stderr, "Error al obtener el nombre de usuario");
        exit(1);
    }

    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        fprintf(stderr, "Error al obtener el directorio actual");
        exit(1);
    }

    fprintf(stdout, GREEN "%s@%s:%s$ " RESET_COLOR, username, hostname, buffer);
}

char *get_Command(void)
{
    prompt(); // Printing prompt for each command reading
    char *command = (char *)malloc(sizeof(char) * 100);
    if (fgets(command, 100, stdin) == NULL) // Reading from stdin
        exit(EXIT_SUCCESS);

    return command; // Returning command as string
}

void execute_Command(char *command)
{
    int back_exec = 0;
    int pipe_flag = 0;
    char *args[64]; // Suponemos un máximo de 64 argumentos
    char *token;
    

    command[strcspn(command,"\n")] = '\0';

    if (strrchr(command,'&')){
        command[strcspn(command,"&")] = '\0';
        back_exec = 1;
    }

    if (strrchr(command,'|')){
        pipe_flag = 1;
    }

    
    int argc = 0;

    // Voy despedazando los comandos por string hasta un espacio
    token = strtok(command, " ");
    while (token != NULL)
    {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    if (argc > 0)
    {   
        if (pipe_flag == 1){
            pipe_invocation(argc,args);
        }
        else if (strcmp(args[0], "cd") == 0) 
        {
            cd(argc, args);
        }
        else if (strcmp(args[0], "clr") == 0){ 
            clr();
        }
        else if (strcmp(args[0],"quit") == 0 ){
            quit();
        }
        else if (strcmp(args[0],"echo") == 0) {
            echo(argc,args);
        }
        else {
            program_invocation(argc,args,back_exec);
        }
    }
}
