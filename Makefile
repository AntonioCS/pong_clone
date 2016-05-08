BIN=pong
TARGET=./build/$(BIN)
CFLAGS=-std=c11 -Wall -Werror -g
LDFLAGS=
LDLIBS=`pkg-config --libs --cflags sdl2`
CC=gcc
SRC_DIR=./src
SRC=$(wildcard $(SRC_DIR)/*.c)
#https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
OBJ_DIR=$(SRC_DIR)/obj
OBJ=$(subst ./src,$(OBJ_DIR), $(patsubst %.c, %.o, $(SRC)))
#http://stackoverflow.com/a/1951111/8715
dir_guard=@mkdir -p $(@D)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(dir_guard)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
$(OBJ):	$(SRC)
	$(dir_guard)
	$(CC) $(CFLAGS) -c $^ $(LDLIBS)
	mv *.o $(OBJ_DIR)

clean:
	rm -rf $(TARGET) ./src/obj/*
