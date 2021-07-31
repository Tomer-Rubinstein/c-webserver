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
#include <stdbool.h>

#define SERVER_PORT 3000
#define MAXLINE 4096
#define SA struct sockaddr

#define ROUTENAME_SIZE 30

void throw(char *msg){
  puts(msg);
  exit(1);
}

char *getHtml(const unsigned char *filename) {
  FILE *fp = fopen(filename, "r");
  char c;
  char *contents = (char *) malloc(sizeof(char) * MAXLINE);
  int allocateCount = 1;
  if(fp == NULL)
    return NULL;

  do {
    c = fgetc(fp);
    if (feof(fp))
      break;
    
    if(strlen(contents) >= MAXLINE*allocateCount)
      contents = (char *) realloc(contents, sizeof(char)*MAXLINE*(++allocateCount));

    strncat(contents, &c, 1);

  } while(1);

  return contents;
}

int main(int argc, char **argv){
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  uint8_t buff[MAXLINE+1];
  uint8_t recvline[MAXLINE+1];

  char *requestedRoute = (char *) malloc(ROUTENAME_SIZE*sizeof(char));
  bool isRoute = false;

  // create a socket
  if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw("could not create socket");

  bzero(&servaddr, sizeof servaddr);
  // setup the address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  if((bind(listenfd, (SA *) &servaddr, sizeof servaddr)) < 0)
    throw("could not bind the server");

  if((listen(listenfd, 10)) < 0)
    throw("listen error");
  printf("listening on port %d...\n", SERVER_PORT);


  for( ; ; ){
    struct sockaddr_in addr;
    socklen_t addr_len;

    fflush(stdout);
    connfd = accept(listenfd, (SA *) NULL, NULL);

    memset(recvline, 0, MAXLINE);

    while((n=read(connfd, recvline, MAXLINE-1)) > 0){
      // fprintf(stdout, "%s\n", recvline);
      requestedRoute = (char *)malloc(sizeof(char)*strlen(recvline));
      for(int i=0; i < strlen(recvline); i++){
        if(recvline[i] == '/')
          isRoute = true;

        if(isRoute && recvline[i] == ' ')
          break;
        
        if(isRoute)
          strncat(requestedRoute, &recvline[i], 1);
      }
      // puts(requestedRoute);

      if(recvline[n-1] == '\n' || isRoute){
        isRoute = false;
        break;
      }
      

      memset(recvline, 0, MAXLINE);
    }



    if(strcmp(requestedRoute,"/") == 0){
      char *content = getHtml("./pages/index.html");
      puts(content);
      if(content == NULL){
        snprintf((char *)buff, sizeof buff, "HTTP/1.0 404\r\n\r\n");
      }else{
        snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", content);
      }
    }else{
      char contentLocation[ROUTENAME_SIZE+6];
      sprintf(contentLocation, "./pages%s.html", requestedRoute);

      char *content = getHtml(contentLocation);
      if(content == NULL){
        snprintf((char *)buff, sizeof buff, "HTTP/1.0 404\r\n\r\n<h1>Page Not Found</h1>");
      }else{
        snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", content);
      }
    }


    

    write(connfd, (char *)buff, strlen((char *)buff));
    close(connfd);
  }

  return 0;
}
