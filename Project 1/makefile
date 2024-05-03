OBJS	= main.o md5.o
SOURCE	= main.c md5.c
HEADER	= msg.h sembuf.h
OUT	= chat
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

md5.o: md5.c
	$(CC) $(FLAGS) md5.c 


clean:
	rm -f $(OBJS) $(OUT)
