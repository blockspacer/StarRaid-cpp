# RakNET: -lpthread -lrt

CC=g++ 
CFLAGS= -c -Isrc/raknet 
LDFLAGS_CLIENT = `sdl-config --cflags --libs` -lSDL_ttf -lSDL_gfx -lSDL_image -Llib/ -lpthread -lrt
LDFLAGS_SERVER = `mysql_config --cflags --libs` -lSDL -Llib/ -lpthread -lrt

#npc client
all: server client

### Server TARGET
SERVER_FILES = $(shell ls src/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' ) $(shell ls src/server/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' )  lib/libRakNetLibStatic.a
server: $(SERVER_FILES)
	$(CC) $(SERVER_FILES) -o bin/server.bin $(LDFLAGS_SERVER)

### Server FILES
lib/server/main.o: src/server/main.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/main.cpp -o lib/server/main.o

lib/server/server.o: src/server/server.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/server.cpp -o lib/server/server.o

lib/server/database.o: src/server/database.cpp
	$(CC) $(CFLAGS) -L lib/server/ src/server/database.cpp -o lib/server/database.o

lib/server/networkClient.o: src/server/networkClient.cpp
	$(CC) $(CFLAGS) src/server/networkClient.cpp -o lib/server/networkClient.o





### Client TARGET
CLIENT_FILES = $(shell ls src/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' ) $(shell ls src/client/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' ) lib/libRakNetLibStatic.a
client: $(CLIENT_FILES)
	$(CC) $(CLIENT_FILES) -o bin/client.bin $(LDFLAGS_CLIENT)

### Client FILES
lib/client/main.o: src/client/main.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/main.cpp -o lib/client/main.o

lib/client/client.o: src/client/client.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/client.cpp -o lib/client/client.o

lib/client/sdlHelper.o: src/client/sdlHelper.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/sdlHelper.cpp -o lib/client/sdlHelper.o

lib/client/gfxImage.o: src/client/gfxImage.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/gfxImage.cpp -o lib/client/gfxImage.o

lib/client/gfxSystem.o: src/client/gfxSystem.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/gfxSystem.cpp -o lib/client/gfxSystem.o

lib/client/gfxFont.o: src/client/gfxFont.cpp
	$(CC) $(CFLAGS) -L lib/client/ src/client/gfxFont.cpp -o lib/client/gfxFont.o





### shared recources
lib/srTimer.o: src/srTimer.cpp
	$(CC) $(CFLAGS) src/srTimer.cpp -o lib/srTimer.o

lib/srConfig.o: src/srConfig.cpp
	$(CC) $(CFLAGS) src/srConfig.cpp -o lib/srConfig.o

lib/srObject.o: src/srObject.cpp
	$(CC) $(CFLAGS) src/srObject.cpp -o lib/srObject.o

lib/srCargo.o: src/srCargo.cpp
	$(CC) $(CFLAGS) src/srCargo.cpp -o lib/srCargo.o





### Cleanup: cant use, otherwise wildecard does not find the files
clean:
	rm lib/*.o lib/server/*.o lib/client/*.o bin/*.bin

#run: server
#	./bin/server

#run: client
#	cd bin && ./client

test:
	@echo $(shell ls src/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' ) $(shell ls src/client/*.cpp | sed 's/cpp/o/g' | sed 's/src/lib/g' )
