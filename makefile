SDL2_INCLUDE=/Library/Frameworks/SDL2.framework/Headers

CC=clang++
CFLAGS=-std=c++14 -I ./src -I $(SDL2_INCLUDE)
LINKFLAGS=-F/Library/Frameworks -framework SDL2

OBJ_DIR = obj
BIN_DIR = bin

all: ElRey
ElRey: $(BIN_DIR)/ElRey

$(OBJ_DIR)/MainWindow.o: src/MainWindow.cpp
	$(CC) $(CFLAGS) -c src/MainWindow.cpp -o $(OBJ_DIR)/MainWindow.o

$(BIN_DIR)/ElRey: $(OBJ_DIR)/MainWindow.o
	$(CC) $(LINKFLAGS) $(OBJ_DIR)/MainWindow.o -o $(BIN_DIR)/ElRey

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/ElRey
