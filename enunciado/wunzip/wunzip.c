/* File: wunzip.c
 *
 * Compilar:
 *   gcc -std=c11 -O2 -Wall -Wextra -o wunzip wunzip.c
 *
 * Uso:
 *   ./wunzip file1 [file2 ...]
 *
 * Formato de entrada:
 *   Secuencias de 4 bytes (uint32_t little-endian) = count,
 *   seguidos por 1 byte = carácter.
 *
 * Ejemplo:
 *   wzip file.txt > file.z
 *   wunzip file.z > file_unzipped.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        /* La prueba espera exactamente esta línea en stdout y rc = 1 */
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int ai = 1; ai < argc; ++ai) {
        const char *fname = argv[ai];
        FILE *f = fopen(fname, "rb");
        if (!f) {
            fprintf(stderr, "wunzip: cannot open file %s\n", fname);
            return 1;
        }

        while (1) {
            uint32_t count;
            unsigned char ch;

            /* leer contador (4 bytes) */
            size_t n = fread(&count, sizeof(count), 1, f);
            if (n == 0) {
                /* EOF limpio */
                break;
            }
            if (n != 1) {
                fprintf(stderr, "wunzip: file %s corrupted (bad count)\n", fname);
                fclose(f);
                return 1;
            }

            /* leer caracter (1 byte) */
            if (fread(&ch, 1, 1, f) != 1) {
                fprintf(stderr, "wunzip: file %s corrupted (missing char)\n", fname);
                fclose(f);
                return 1;
            }

            /* escribir 'count' veces el carácter */
            for (uint32_t i = 0; i < count; i++) {
                if (fputc(ch, stdout) == EOF) {
                    perror("write");
                    fclose(f);
                    return 1;
                }
            }
        }

        if (ferror(f)) {
            fprintf(stderr, "wunzip: error reading file %s\n", fname);
            fclose(f);
            return 1;
        }

        fclose(f);
    }

    return 0;
}
