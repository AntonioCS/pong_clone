BIN=pong
BUILD_DIR=./build
TARGET=$(BUILD_DIR)/$(BIN)
CFLAGS=-std=c11 -Wall -Werror -g -O0
LDFLAGS=
LDLIBS=`pkg-config --libs --cflags sdl2 SDL2_ttf`
CC=gcc
SRC_DIR=./src
SRC=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(SRC_DIR)/*.h)
#https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
OBJ_DIR=$(SRC_DIR)/obj
OBJ=$(subst ./src,$(OBJ_DIR), $(patsubst %.c, %.o, $(SRC)))
#http://stackoverflow.com/a/1951111/8715
dir_guard=@mkdir -p $(@D)

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(dir_guard)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
#copy all the resources to the build dir so that the
#executable can access everything in ./resources/<RESOURCE>
#added the @ to suppress the echoing
	@cp -n -R ./resources $(BUILD_DIR)/resources

#To cause recompile when .h files are change
$(OBJ):	$(SRC) $(HEADERS)
	$(dir_guard)
	$(CC) $(CFLAGS) -c $(SRC) $(LDLIBS)
#Put all the object files in the correct directory
	@mv *.o $(OBJ_DIR)

clean:
	rm -rf $(TARGET) ./src/obj/*
