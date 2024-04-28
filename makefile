CC=gcc
CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

All: server client

all: $(All)

server: server.o network.o ipc.o game.o utils_v1.o 
	$(CC) $(CCFLAGS) -o server server.o network.o ipc.o game.o utils_v1.o

client: client.o network.o ipc.o game.o utils_v1.o 
	$(CC) $(CCFLAGS) -o client client.o network.o ipc.o game.o utils_v1.o 

client.o: client.c network.h game.h utils_v1.h
	$(CC) $(CCFLAGS) -c client.c

server.o: server.c network.h game.h utils_v1.h
	$(CC) $(CCFLAGS) -c server.c

network.o: network.c network.h utils_v1.h structures.h
	$(CC) $(CCFLAGS) -c network.c

ipc.o: ipc.c ipc.h utils_v1.h structures.h
	$(CC) $(CCFLAGS) -c ipc.c

game.o: game.c game.h utils_v1.h structures.h
	$(CC) $(CCFLAGS) -c game.c

utils_v1.o: utils_v1.c utils_v1.h
	$(CC) $(CCFLAGS) -c utils_v1.c

clear:
	clear

clean:
	rm -f *.o
	rm -f server
	rm -f client