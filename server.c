#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SERVER_PORT 3000
#define MAXLINE 4096
#define SA struct sockaddr

void throw(char *msg){
  puts(msg);
  exit(1);
}

char *getHtml(char *filename){
  FILE *fptr = fopen(filename, "r");
  char *line = (char *) malloc(sizeof(char)*MAXLINE);
  //char *contents = (char *) malloc(sizeof(char) * MAXLINE);
  size_t len = MAXLINE;
  ssize_t read;
  //int allocatedCount = 1;

  if(fptr == NULL)
    return NULL;

  while ((read = getline(&line, &len, fptr)) != -1) {
    // if(strlen(contents)+read > allocatedCount*MAXLINE)
    //   contents = (char *)realloc(contents, sizeof(char)*(++allocatedCount));
    puts(line);
    //strncat(contents, line, strlen(line));
  }

  //puts(contents);
  fclose(fptr);
  return NULL;
}

int main(int argc, char **argv){
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  uint8_t buff[MAXLINE+1];
  uint8_t recvline[MAXLINE+1];

  if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw("could not create socket");

  bzero(&servaddr, sizeof servaddr);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  if((bind(listenfd, (SA *) &servaddr, sizeof servaddr)) < 0)
    throw("could not bind the server");

  if((listen(listenfd, 10)) < 0)
    throw("listen error");

  printf("listening on port %d...\n", SERVER_PORT);
  for( ; ; ){
    fflush(stdout);
    connfd = accept(listenfd, (SA *)NULL, NULL);

    memset(recvline, 0, MAXLINE);

    snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", "<p>asfsf</p>");

    write(connfd, (char *)buff, strlen((char *)buff));
    close(connfd);
  }

  return 0;
}
