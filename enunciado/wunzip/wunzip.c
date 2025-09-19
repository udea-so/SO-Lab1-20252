#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  * soydz.com
*/
void read_file(char* name_file, char **buffer, long *size_buffer);
void write_file(char *output, long count_bytes);
void decompress_rle(char* buffer, char** output, long size_buffer, long *count_bytes);

int main(int argc, char* argv[]) {

  if (argc < 2 ){
    printf("wunzip: file1 [file2 ...]\n");
    exit(1);
  }

  for (int i = 1; i < argc; i++) {
    char* buffer;
    char* output;
    long size_buffer = 0;
    long count_bytes = 0;

    read_file(argv[i], &buffer, &size_buffer);

    decompress_rle(buffer, &output, size_buffer, &count_bytes);

    write_file(output, count_bytes);

    free(buffer);
    free(output);
  }

  return 0;
}

void read_file(char* name_file, char **buffer, long *size_buffer){
  FILE *fp = fopen(name_file, "rb");

  if(!fp){
    printf("wuzip: cannot open file\n");
  }

  // calcula el tamaño en bytes del archivo
  fseek(fp, 0, SEEK_END);
  *size_buffer = ftell(fp);
  rewind(fp);

  *buffer = malloc(*size_buffer + 1);

  if (!*buffer) {
    perror("malloc");
    fclose(fp);
    exit(1);
  }

  // lee el archivo
  fread(*buffer, 1, *size_buffer, fp);

  fclose(fp);
}

void write_file(char *output, long count_bytes) {
  fwrite(output, 1, count_bytes, stdout);
}

void decompress_rle(char* buffer, char** output, long size_buffer, long *count_bytes) {

  // calcula el tamaño del archivo descomprimido
  long size_input = 0;

  for (size_t i = 0; i < size_buffer; i += 5) {
    int count;
    memcpy(&count, buffer + i, sizeof(int));
    size_input += count;
  }

  *output = malloc(size_input);
  int pos_output = 0;
  
  for(size_t i = 0; i < size_buffer; i += 5) {

    int count;
    memcpy(&count, buffer + i, sizeof(int));

    char c = buffer[i + 4];

    for (int j = 0; j < count; j++) {
      (*output)[pos_output++] = c;
    }
  
  }
  
  (*count_bytes) = pos_output;
}
