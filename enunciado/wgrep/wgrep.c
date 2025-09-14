#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* soydz.com
 * */

void leerArchivo(char *name_archivo, char *palabra);
int buscarPalabra(char *palabra, char *cadena);


int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);

  } else if(argc == 2) {
    leerArchivo(NULL, argv[1]);

  } else if (argc > 2) {
 
    for(int i = 2; i < argc; i++) {
      leerArchivo(argv[i], argv[1]);
    }    
  }

  return 0;
}

void leerArchivo(char *name_archivo, char *palabra) {

  FILE *fp;
  
  if (name_archivo) {
    fp = fopen(name_archivo, "r");

  } else {
    fp = stdin;
  }

  if(!fp) {
    printf("wgrep: cannot open file\n");
    exit(1);
  }

  char *linea = NULL;
  size_t len = 0;

  while((getline(&linea, &len, fp)) != -1) {
    if (buscarPalabra(palabra, linea) == 0) {
      printf("%s", linea);
    }
  }
  
  free(linea);
  fclose(fp);
}


int buscarPalabra(char *palabra, char* cadena) {
  if (strstr(cadena, palabra)) {
    return 0;
  }
  return 1; 
}
