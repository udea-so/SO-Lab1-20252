/* File: wzip.c
 *
 * Compilar:
 *   gcc -std=c11 -O2 -Wall -Wextra -o wzip wzip.c
 *
 * Uso:
 *   ./wzip file1 [file2 ...]
 *
 * Formato de salida:
 *   Para cada run de bytes iguales: 4 bytes little-endian (uint32_t) = count,
 *   seguido del byte (1 byte).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        /* La prueba espera exactamente esta línea en stdout y rc = 1 */
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    unsigned char curr = 0;        /* byte actual del run */
    uint32_t count = 0;            /* contador del run (4 bytes) */
    int have_curr = 0;             /* si tenemos un byte inicializado */

    for (int ai = 1; ai < argc; ++ai) {
        const char *fname = argv[ai];
        FILE *f = fopen(fname, "rb");
        if (!f) {
            /* Si no se puede abrir un archivo: salir con error */
            fprintf(stderr, "wzip: cannot open file %s\n", fname);
            return 1;
        }

        int ch;
        while ((ch = fgetc(f)) != EOF) {
            unsigned char b = (unsigned char) ch;
            if (!have_curr) {
                curr = b;
                count = 1;
                have_curr = 1;
            } else {
                if (b == curr) {
                    /* Mismo byte: aumentar contador */
                    /* proteger contra overflow (muy improbable en prueba),
                       pero si se alcanza 0xFFFFFFFF lo volcamos y seguimos */
                    if (count == UINT32_MAX) {
                        /* escribir run actual */
                        uint32_t out_count = count;
                        if (fwrite(&out_count, sizeof(out_count), 1, stdout) != 1) {
                            fclose(f);
                            perror("write");
                            return 1;
                        }
                        if (fwrite(&curr, 1, 1, stdout) != 1) {
                            fclose(f);
                            perror("write");
                            return 1;
                        }
                        count = 1; /* reiniciar con el byte actual */
                    } else {
                        ++count;
                    }
                } else {
                    /* byte distinto: volcar run anterior */
                    uint32_t out_count = count;
                    if (fwrite(&out_count, sizeof(out_count), 1, stdout) != 1) {
                        fclose(f);
                        perror("write");
                        return 1;
                    }
                    if (fwrite(&curr, 1, 1, stdout) != 1) {
                        fclose(f);
                        perror("write");
                        return 1;
                    }
                    curr = b;
                    count = 1;
                }
            }
        }

        if (ferror(f)) {
            fprintf(stderr, "wzip: error reading file %s\n", fname);
            fclose(f);
            return 1;
        }

        fclose(f);
    }

    /* Al terminar todos los archivos, si hay un run pendiente lo volcamos */
    if (have_curr) {
        uint32_t out_count = count;
        if (fwrite(&out_count, sizeof(out_count), 1, stdout) != 1) {
            perror("write");
            return 1;
        }
        if (fwrite(&curr, 1, 1, stdout) != 1) {
            perror("write");
            return 1;
        }
    }

    return 0;
}
