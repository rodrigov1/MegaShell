#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "clr.h"

void clr(void){
    fprintf(stdout,"\033[H\033[J"); // Limpia la pantalla y coloca el cursor en la parte superior izquierda
    return;
}
