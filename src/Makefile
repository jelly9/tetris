INCLUDE=../include
CFLAGS= -Wall -g
CC=gcc

tetris:tetris.o keyboard.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o:%.c
	$(CC) -o $@ $< -c -I$(INCLUDE)

.PHONY:clean
clean:
	rm *.o tetris
