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

#include "server.h"

#define SERVER_PORT 3000
#define SA struct sockaddr

int main(int argc, char **argv){
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  uint8_t recvline[MAXLINE+1];

  char *requestedRoute = (char *) malloc(ROUTENAME_SIZE*sizeof(char));
  bool isRoute = false;

  char tempChar;

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
    fflush(stdout);
    connfd = accept(listenfd, (SA *) NULL, NULL);

    memset(recvline, 0, MAXLINE);

    while((n=read(connfd, recvline, MAXLINE-1)) > 0){
      requestedRoute = (char *) malloc(sizeof(char)*strlen((char *)recvline));
      for(int i=0; i < strlen((char *)recvline); i++){
        if(recvline[i] == '/')
          isRoute = true;

        if(isRoute && recvline[i] == ' ')
          break;
        
        if(isRoute){
          tempChar = (char)recvline[i];
          strncat(requestedRoute, &tempChar, 1);
        }
      }

      if(recvline[n-1] == '\n' || isRoute){
        isRoute = false;
        break;
      }

      memset(recvline, 0, MAXLINE);
    }

    servePage(requestedRoute);

    write(connfd, (char *)buff, strlen((char *)buff));
    close(connfd);
  }

  return 0;
}
