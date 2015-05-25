CC=g++ 
CFLAGS=-Wall -c
LDFLAGS_CLIENT=`sdl-config --cflags --libs`
LDFLAGS_SERVER=-lSDL

#npc client
all: server client

### Server TARGET
server: lib/server/main.o lib/server/server.o lib/srTimer.o lib/srNetwork.o
	$(CC) lib/server/main.o lib/server/server.o lib/srTimer.o lib/srNetwork.o -o bin/server $(LDFLAGS_SERVER)


### Server FILES
lib/server/main.o: src/server/main.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/main.cpp -o lib/server/main.o

lib/server/server.o: src/server/server.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/server.cpp -o lib/server/server.o






### Client TARGET
client: lib/client/main.o
	$(CC) lib/client/main.o -o bin/client $(LDFLAGS_CLIENT)


### Client FILES
lib/client/main.o: src/client/main.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/main.cpp -o lib/client/main.o






### Shared Stuff
lib/srNetwork.o: src/srNetwork.cpp
	$(CC) $(CFLAGS) src/srNetwork.cpp -o lib/srNetwork.o

lib/srTimer.o: src/srTimer.cpp
	$(CC) $(CFLAGS) src/srTimer.cpp -o lib/srTimer.o





### Cleanup
clean:
	rm lib/server/* lib/client/* bin/server bin/client

#run: server
#	./bin/server

#run: client
#	cd bin && ./client