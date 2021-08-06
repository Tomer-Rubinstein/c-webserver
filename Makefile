serveFiles: serveFiles.c
	gcc -Wall -pedantic serveFiles.c -c -o serveFiles.o

server: server.c serveFiles.o
	gcc -Wall -pedantic server.c serveFiles.o -o server
