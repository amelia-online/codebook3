CFLAGS = -fsanitize=undefined -fsanitize=address -g
CC = gcc
OBJS = token.o codebook.o

cb3exe: main.o token.o codebook.o
	$(CC) $(CFLAGS) main.o $(OBJS) -o cb3exe

tests: tests.o codebook.o
	$(CC) $(CFLAGS) tests.o codebook.o -o tests

tests.o: src/tests.c
	$(CC) $(CFLAGS) -c src/tests.c

main.o: src/main.c token.o
	$(CC) $(CFLAGS) -c src/main.c

token.o: src/token.c headers/token.h
	$(CC) $(CFLAGS) -c src/token.c

codebook.o: src/codebook.c headers/codebook.h
	$(CC) $(CFLAGS) -c src/codebook.c

clean:
	rm *.o ./cb3exe ./tests
