#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  * soydz.com
*/


void read_file(char* name_file, char **buffer, long *size_buffer);
void write_file(unsigned char *output, size_t output_size);
void compression_rle(char* buffer, long size, char *last_char, int *last_count, unsigned char **output, size_t *output_size);

int main(int argc, char **argv) {

  if(argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    exit(1);
  }
  // ultimo caracter leido
  char last_char = 0;
  // veces que se repite el ultimo caracter
  int last_count = 0;
  
  for (int i = 1; i < argc; i++) {
    char *buffer;
    long size_buffer;

    read_file(argv[i],&buffer, &size_buffer);

    unsigned char *output;
    size_t output_size;

    compression_rle(buffer, size_buffer, &last_char, &last_count, &output, &output_size);

    write_file(output, output_size);

    free(buffer);
    free(output);
  }

  // como no hay un caracter que dispare el guardado, se hace manual
  if (last_count > 0) {
    unsigned char final[5];
    int count = last_count;

    // guarda como binario las repeticiones del caracter
    memcpy(final, &count, sizeof(int));

    // el caracter al final del arreglo
    final[4] = last_char;
    fwrite(final, 1, 5, stdout);

  }
    return 0;
}


void read_file(char* name_file, char **buffer, long *size_buffer) {

  FILE *fp;

  if (name_file) {
    fp = fopen(name_file, "rb");
  }

  if (!fp) {
    printf("wzip: cannot open file\n");
  }

  // Calcular el tamaño en bytes del archivo
  fseek(fp, 0, SEEK_END);
  *size_buffer = ftell(fp);
  rewind(fp);

  *buffer = malloc(*size_buffer + 1);

  // si falla la reserva de memoria, se retorna un error
  if (!*buffer) {
        perror("malloc");
        fclose(fp);
        exit(1);
    }

  // lee el archivo
  fread(*buffer, 1, *size_buffer, fp);
  fclose(fp);  
}


void write_file(unsigned char *output, size_t output_size) {
  fwrite(output, 1, output_size, stdout);
}

void compression_rle(char *buffer, long size, char *last_char, int *last_count, unsigned char **output, size_t *output_size) {

    // 5 por que el entero es de 4B y 1B del caracter
    *output = malloc(size * 5);
    size_t j = 0;

    for (long i = 0; i < size; i++) {
        char c = buffer[i];

        if (c == *last_char) {
            (*last_count)++;
        } else {
            if (*last_count > 0) {
                // escribir bloque anterior
                int count = *last_count;
                
                // copia en binario los 4B de count dentro del buffer output en la posicion j.
                memcpy(&(*output)[j], &count, sizeof(int));
                j += sizeof(int);

                // agrega el caracter
                (*output)[j++] = *last_char;
            }
            // no hubo caracter repetido
            *last_char = c;
            *last_count = 1;
        }
    }

    *output_size = j;
}
