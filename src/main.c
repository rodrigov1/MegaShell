#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

int main()
{
    char input[1024];
    char *args[64]; // Suponemos un máximo de 64 argumentos
    char *token;
    char buffer[PATH_MAX];
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];

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

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break; // Salir si se alcanza EOF
        }

        // Eliminar el salto de línea al final del comando
        size_t input_length = strlen(input);
        if (input_length > 0 && input[input_length - 1] == '\n')
        {
            input[input_length - 1] = '\0';
        }

        // Voy guardando los comandos por tokens para saber que hacer
        int argc = 0;
        token = strtok(input, " ");
        while (token != NULL)
        {
            args[argc] = token;
            argc++;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        // Comando 'cd'
        if (argc > 0 && strcmp(args[0], "cd") == 0)
        {
            if (argc == 1)
            {
                // Sin argumentos, mostrar el directorio actual
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    fprintf(stderr,"Error al obtener el directorio actual");
                }
            }
            else if (argc == 2)
            {
                // Cambiar al directorio anterior
                if (strcmp(args[1], "-") == 0)
                {
                    char *old_pwd = getenv("OLDPWD");
                    if (old_pwd != NULL)
                    {
                        chdir(old_pwd);
                    }
                    else
                    {
                        fprintf(stderr,"Variable de entorno OLDPWD no definida\n");
                    }
                }
                else
                {
                }
            }
            else if (argc == 3)
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
                        perror("Error al obtener el directorio actual");
                    }
                }
                else
                {
                    perror("Error al cambiar de directorio");
                }
            }
        }
        else
        {
            // Aca irian los otros comandos
        }
    }

    printf("Saliendo de myshell.\n");
    return 0;
}
