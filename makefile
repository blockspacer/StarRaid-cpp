CC=g++ 
CFLAGS= -c
LDFLAGS_CLIENT=`sdl-config --cflags --libs`
LDFLAGS_SERVER=-lSDL

#npc client
all: server client

### Server TARGET
server: lib/server/main.o lib/server/server.o lib/srTimer.o lib/srNetwork.o lib/srConfig.o lib/srTypes.o
	$(CC) lib/server/main.o lib/server/server.o lib/srTimer.o lib/srNetwork.o lib/srConfig.o lib/srTypes.o -o bin/server.bin $(LDFLAGS_SERVER)


### Server FILES
lib/server/main.o: src/server/main.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/main.cpp -o lib/server/main.o

lib/server/server.o: src/server/server.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/server.cpp -o lib/server/server.o





### Client TARGET
client: lib/client/main.o lib/client/client.o lib/srTimer.o lib/srNetwork.o lib/srConfig.o lib/srTypes.o
	$(CC) lib/client/main.o lib/client/client.o lib/srTimer.o lib/srNetwork.o lib/srConfig.o lib/srTypes.o -o bin/client.bin $(LDFLAGS_CLIENT)


### Client FILES
lib/client/main.o: src/client/main.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/main.cpp -o lib/client/main.o

lib/client/client.o: src/client/client.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/client.cpp -o lib/client/client.o





### Shared Stuff
lib/srNetwork.o: src/srNetwork.cpp
	$(CC) $(CFLAGS) src/srNetwork.cpp -o lib/srNetwork.o

lib/srTimer.o: src/srTimer.cpp
	$(CC) $(CFLAGS) src/srTimer.cpp -o lib/srTimer.o

lib/srConfig.o: src/srConfig.cpp
	$(CC) $(CFLAGS) src/srConfig.cpp -o lib/srConfig.o

lib/srTypes.o: src/srTypes.cpp
	$(CC) $(CFLAGS) src/srTypes.cpp -o lib/srTypes.o





### Cleanup
clean:
	rm lib/*.o lib/server/*.o lib/client/*.o bin/*.bin

#run: server
#	./bin/server

#run: client
#	cd bin && ./client