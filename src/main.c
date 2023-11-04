#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "manage_commands.h"
#include "cd.h"
#include "clr.h"
/*
1. Command line prompt (myshell debe contar con un prompt que contenga el camino al directorio actual) [LISTO]

Por ejemplo, para el home: username@hostname:~$

2. Internal commands (myshell debe soportar los siguientes comandos internos) [ LISTO ]

    cd > directorio > : cambia el directorio actual a <directorio>. Si <directorio> no está presente, reporta el directorio actual.
    Si el directorio no existe se debe imprimir un error apropiado. Además, este comando debe cambiar la variable de entorno PWD.
    Este comando debe soportar la opción cd -, que retorna al último directorio de trabajo (OLDPWD). [LISTO]

    clr: limpia la pantalla [LISTO]

    echo <comentario|env var> : muestra <comentario> en la pantalla seguido por una línea nueva. (múltiples espacios/tabs pueden ser reducidos a un espacio).

    quit : cierra myshell [LISTO]
*/

int main(int argc, char const *argv[])
{
	if(argc>=2){
		FILE* file = fopen(argv[1],"r");
		if(file!=NULL){
			char* line = (char*)malloc(sizeof(char)*1024);
			while(fgets(line,1024,file)!=NULL){
				execute_Command(line);
			}
			fclose(file);
			free(line);
			exit(0);
		}
		else{
			fprintf(stderr,"Error opening file\n");
			exit(-1);
		}
	}

	while(1){
		char* command;
		command = get_Command();
		execute_Command(command);
	}

	return 0;
}

/*
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

        int argc = 0;
        // Voy despedazando los comandos por string hasta un espacio
        token = strtok(input, " ");
        while (token != NULL)
        {
            args[argc] = token;
            argc++;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        
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
                        fprintf(stdout,"%s\n", cwd);
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
            else if (strcmp(args[0], "clr") == 0) // Comando clr
            {
                fprintf(stdout,"\033[H\033[J"); // Limpia la pantalla y coloca el cursor en la parte superior izquierda
            } 
            else if (strcmp(args[0],"quit") == 0) // Comando quit
            {
                break;
            } 
            else if (strcmp(args[0],"echo") == 0) // Muestra comentario o variable de entorno por pantalla
            {
                if (strchr(args[1],'$')!=NULL) // 
                {
                    char* string = strchr(args[1],'$');
                    char* var = string+1;
                    if (getenv(var)!=NULL)
                        fprintf(stdout,"%s\n",getenv(var));
                    else
                        fprintf(stdout,"Error con la variable de entorno\n");
                } else {
                    int i = 1;
                    while (args[i] != NULL){
                        fprintf(stdout,"%s ", args[i]);
                        i++;
                    }
                    fprintf(stdout,"\n");
                }
            }
        }
    }

    //printf("Saliendo de myshell.\n");
    return 0;
}*/
