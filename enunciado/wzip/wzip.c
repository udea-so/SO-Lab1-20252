#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    char prev;
    int count = 0;
    int firstChar = 1;

    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f == NULL) {
            fprintf(stderr, "wzip: cannot open file\n");
            exit(1);
        }

        char c;
        while ((c = fgetc(f)) != EOF) {
            if (firstChar) {
                prev = c;
                count = 1;
                firstChar = 0;
            } else if (c == prev) {
                count++;
            } else {
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&prev, sizeof(char), 1, stdout);
                prev = c;
                count = 1;
            }
        }

        fclose(f);
    }

    if (!firstChar) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&prev, sizeof(char), 1, stdout);
    }

    return 0;
}
