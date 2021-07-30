#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXLINE 4096

char *getHtml(char *filename) {
    FILE *file;
    char *code = (char *)malloc(2000 * sizeof(char));
    file = fopen(filename, "r");
	if(file == NULL){
		printf("error\n");
		return NULL;
	}
    do {
      *code++ = (char)fgetc(file);
    } while(*code != EOF);
    return code;
}



int main(){
	char file[20] = "homepage.html\0";
	printf("%s\n", getHtml(file));
	return 1;
}
