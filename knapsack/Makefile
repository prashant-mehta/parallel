
CC = gcc
OBJS = timer.o
FLAGS = -O3 -fopenmp
SEQFLAGS = -O3 

EXEC = knapSeq knap knapBSeq knapB

all: $(EXEC)

knapSeq: knapSeq.c timer.o
	$(CC)  $(SEQFLAGS) -o knapSeq knapSeq.c $(OBJS) 

knap: knap.c timer.o
	$(CC)  $(FLAGS) -o knap knap.c $(OBJS) 

knapBSeq: knapBSeq.c timer.o
	$(CC)  $(SEQFLAGS) -o knapBSeq knapBSeq.c $(OBJS) 

knapB: knapB.c timer.o
	$(CC)  $(FLAGS) -o knapB knapB.c $(OBJS) 

timer.o: timer.c
	$(CC) -o $@ -c timer.c

clean:
	rm -f $(EXEC) $(OBJS)
