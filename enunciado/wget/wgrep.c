#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // Mensaje esperado por los tests
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char *searchterm = argv[1];
    char line[MAX_LINE];

    // Caso: sin archivos → leer desde stdin
    if (argc == 2) {
        while (fgets(line, sizeof(line), stdin)) {
            if (strstr(line, searchterm) != NULL) {
                printf("%s", line);
            }
        }
        return 0;
    }

    // Caso: leer archivos
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, searchterm) != NULL) {
                printf("%s", line);
            }
        }
        fclose(fp);
    }

    return 0;
}



