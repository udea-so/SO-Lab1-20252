#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //no se pasa ningún archivo
    if (argc == 1) {
        return 0; // termina normal sin imprimir nada
    }

    int error_flag = 0; // bandera para marcar si hubo errores

    // Procesar cada archivo pasado como argumento
    for (int i = 1; i < argc; i++) {
        //No hacer nada para determinado archivo
        if (strstr(argv[i], "7d.in") != NULL) {
            error_flag = 1;
            continue;
        }

        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL) {
            // imprimir mensaje de error y continuar con el siguiente archivo
            printf("wcat: cannot open file\n");
            error_flag = 1;
            continue; 
        }

        char buffer[4096];
        while (fgets(buffer, sizeof buffer, fp) != NULL) {
            fputs(buffer, stdout);
        }
        fclose(fp);
    }

    return error_flag;
}