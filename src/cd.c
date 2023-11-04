#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "cd.h"

void cd(int argc, char *args[])
{
    if (argc > 0)
    {
        if (strcmp(args[0], "cd") == 0) // Comando 'cd'
        {
            if (argc == 1)
            {
                // Sin argumentos, mostrar el directorio actual
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    fprintf(stdout, "%s\n", cwd);
                }
                else
                {
                    fprintf(stderr, "Error al obtener el directorio actual");
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
                        fprintf(stderr, "Variable de entorno OLDPWD no definida\n");
                    }
                }
                else
                {
                }
            }
            else if (argc == 3)
            {
                if (chdir(args[2]) == 0)
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
                    fprintf(stderr, "Error al cambiar de directorio\n");
                }
            }
        }
    }
}