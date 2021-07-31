#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ROUTENAME_SIZE 30
#define MAXLINE 4096

uint8_t buff[MAXLINE+1];

/*
  return file contents as a string from a file
*/
char *getHtml(char *filename) {
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

/* 
  if 404.html exists in the `/pages` directory => serve 404.html
  if not, serve the default 404 page
*/
void pageNotFound(){
  char *notFoundPage = getHtml("./pages/404.html");

  if(notFoundPage == NULL)
    snprintf((char *)buff, sizeof buff, "HTTP/1.0 404\r\n\r\n");
  else
    snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", notFoundPage);
}

/*
  serves a html file from `/pages` by the requested route.
  i.e, www.somedomain.com/about will search for about.html in the pages directory.
  the default route `/` is called `index.html` in the pages directory.
*/
void servePage(char *requestedRoute){
  char *indexPage = getHtml("./pages/index.html");
  char *requestedPage;
  
  if(strcmp(requestedRoute, "/") == 0){
    // show index page
    if(indexPage == NULL)
      pageNotFound();
    else
      snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", indexPage);
  }else{
    char *contentLocation = (char *)malloc(sizeof(char)*ROUTENAME_SIZE);
    int allocatedCount = 1;
    strncat(contentLocation, "./pages", 8);
    char ext[6] = ".html\0";

    for(int i=0; i < strlen(requestedRoute)+strlen(ext); i++){
      if(strlen(contentLocation) >= ROUTENAME_SIZE*allocatedCount)
        contentLocation = (char *) realloc(contentLocation, sizeof(char)*ROUTENAME_SIZE*(++allocatedCount));
      if(!strstr(contentLocation, requestedRoute))
        strncat(contentLocation, &requestedRoute[i], 1);
      else
        strncat(contentLocation, &ext[i-strlen(requestedRoute)], 1);
    }

    puts(contentLocation);
    requestedPage = getHtml(contentLocation);

    if(requestedPage == NULL)
      pageNotFound();
    else
      snprintf((char *)buff, sizeof buff, "HTTP/1.0 200 OK\r\n\r\n%s", requestedPage);
  }
}
