#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/echo.h"

void echo(int argc, char *args[])
{
    if (strchr(args[1], '$') != NULL) //
    {
        char *var = strchr(args[1], '$')+1;
        if (getenv(var) != NULL)
            fprintf(stdout, "%s\n", getenv(var));
        else
            fprintf(stdout, "Error con la variable de entorno\n");
    }
    else
    {
        int i = 1;
        while (args[i] != NULL)
        {
            fprintf(stdout, "%s ", args[i]);
            i++;
        }
        fprintf(stdout, "\n");
    }
}
