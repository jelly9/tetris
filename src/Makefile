INCLUDE=../include
CC=gcc

tetris:tetris.o keyboard.o
	$(CC) -o $@ $^

%.o:%.c
	$(CC) -o $@ $< -c -I$(INCLUDE)

.PHONY:clean
clean:
	rm *.o tetris
