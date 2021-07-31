#include "serveFiles.c"

void throw(char *msg){
  printf("SERVER ERROR: %s\n", msg);
  exit(1);
}
