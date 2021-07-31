serveFiles: serveFiles.c
	gcc -Wall -pedantic serveFiles.c -c -o serveFiles.o

server: server.c serveFiles.o serverHeader.o
	gcc -Wall -pedantic server.c serveFiles.o serverHeader.o -o server
