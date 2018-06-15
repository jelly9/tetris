INCLUDE=../include
LIB=../lib
CFLAGS=keyboard
SRC=tetris.c
OBJ=$(shell echo $(SRC) | sed s/\.c/\.o/g)
BIN=tetris
CC=gcc

$(BIN):$(OBJ)
	$(CC) $^ -o $@ -L$(LIB) -l$(CFLAGS)
	rm $(OBJ)

%.o:%.c
	$(CC) -c $< -o $@ -I$(INCLUDE)

.PHONY:clean
clean:
	rm $(BIN)
