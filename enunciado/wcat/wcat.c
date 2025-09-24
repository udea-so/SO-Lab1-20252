#include <stdio.h>
#include <stdlib.h>

#define TAM 101   // tamaño del buffer de lectura

int main(int argc, char *argv[]) {
    FILE *fileIn;
    char buffer[TAM];

    // Si no se pasan archivos, no hace nada (solo termina)
    if (argc == 1) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        // Abrir archivo
        fileIn = fopen(argv[i], "r");
        if (fileIn == NULL) {
            printf("wcat: cannot open file\n");
            return 1;
        }

        // Procesar archivo
        while (fgets(buffer, TAM, fileIn) != NULL) {
            printf("%s", buffer);
        }

        fclose(fileIn);
    }

    return 0;
}



