#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h> // Para PATH_MAX

/*
1. Command line prompt (myshell debe contar con un prompt que contenga el camino al directorio actual) [LISTO]

Por ejemplo, para el home: username@hostname:~$

2. Internal commands (myshell debe soportar los siguientes comandos internos)

    cd > directorio > : cambia el directorio actual a <directorio>. Si <directorio> no está presente, reporta el directorio actual. Si el directorio no existe se debe imprimir un error apropiado. Además, este comando debe cambiar la variable de entorno PWD. Este comando debe soportar la opción cd -, que retorna al último directorio de trabajo (OLDPWD).

    clr: limpia la pantalla

    echo <comentario|env var> : muestra <comentario> en la pantalla seguido por una línea nueva. (múltiples espacios/tabs pueden ser reducidos a un espacio).

    quit : cierra myshell
*/
void command_cd(int, char *arr[64]);

int main()
{
    char buffer[PATH_MAX];
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    char input[1024];
    char *args[64];
    char *token;

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

    while (1)
    {
        if (getcwd(buffer, sizeof(buffer)) == NULL)
        {
            fprintf(stderr, "Error al obtener el directorio actual");
            exit(1);
        }

        printf("%s@%s:%s$ ", username, hostname, buffer);

        char command[1024];

        if (fgets(command, sizeof(command), stdin) != NULL)
        {
            int argc = 0;
            token = strtok(command, " ");
            while (token != NULL)
            {
                args[argc] = token;
                argc++;
                token = strtok(NULL, " ");
            }
            args[argc] = NULL; // Marco el final
            
            if (strcmp(args[0], "cd") == 0)
            {
                switch (argc)
                {
                case 1: // No arguments, show actual directory
                    char cwd[1024];
                    if (getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        printf("%s\n", cwd);
                    }
                    else
                    {
                        fprintf(stderr, "Error al obtener el directorio actual");
                    }
                    break;
                case 2: // 1 Argument, it could be "-" or ">"
                    if (strcmp(args[1], "-") == 0)
                    {
                        char *old_pwd = getenv("OLDPWD");
                        if (old_pwd != NULL)
                        {
                            chdir(old_pwd);
                        }
                        else
                        {
                            printf("Variable de entorno OLDPWD no definida\n");
                        }
                    }
                    else if (strcmp(args[1], ">") == 0)
                    {
                        if (chdir(args[1]) == 0)
                        {
                            char cwd[1024];
                            if (getcwd(cwd, sizeof(cwd)) != NULL)
                            {
                                setenv("OLDPWD", getenv("PWD"), 1); // Actualizar OLDPWD
                                setenv("PWD", cwd, 1);              // Actualizar PWD
                            }
                            else
                            {
                                fprintf(stderr, "Error al obtener el directorio actual");
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Error al cambiar al directorio especificado");
                        }
                    }
                    break;
                }
            }
        }
    }

    // printf("\nSaliendo del shell.\n");
    return 0;
}

void command_cd(int argc, char *args[64])
{
}
