#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "../include/manage_commands.h"
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
		FILE* file = fopen(argv[1],"r"); // Read a file line by line and execute it as command until EOF
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

