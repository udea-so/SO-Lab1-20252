#include <stdio.h>
#include <stdlib.h>

/*
  * soydz.com
*/

void read_file(char* name_file, char** buffer, long *size_buffer);
void write_file(char* buffer, long size_buffer);

int main (int argc, char* argv[]) {

  if (argc < 2) {
    return 0;
  }

  for(int i = 1; i < argc; i++) { 
    char *buffer;
    long size_buffer = 0;

    read_file(argv[i], &buffer, &size_buffer);

    write_file(buffer, size_buffer);

    free(buffer);
  }
  return 0;
}


void read_file(char* name_file, char** buffer, long *size_buffer) {

  FILE *fp = fopen(name_file, "r");

  if(!fp) {
    printf("wcat: cannot open file\n");
    exit(1);
  }

  // calcula el tamaño del archivo
  fseek(fp, 0, SEEK_END);
  *size_buffer = ftell(fp);
  rewind(fp);

  // reserva memoria
  *buffer = malloc(*size_buffer + 1);

  if(!*buffer) {
    printf("Error reservando memoria");
    fclose(fp);
    exit(1);
  }

  // lee el archivo
  fread(*buffer, 1, *size_buffer, fp);
  fclose(fp);
}

void write_file(char *buffer, long size_buffer) {
  fwrite(buffer, 1, size_buffer, stdout);
}
