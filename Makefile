CFLAGS = -fsanitize=undefined -fsanitize=address -g
CC = gcc
OBJS = token.o codebook.o

forge: main.o token.o codebook.o
	$(CC) $(CFLAGS) main.o $(OBJS) -o forge

tests: tests.o codebook.o
	$(CC) $(CFLAGS) tests.o codebook.o -o tests

tests.o: src/tests.c headers/cbtesting.h
	$(CC) $(CFLAGS) -c src/tests.c

main.o: src/main.c token.o
	$(CC) $(CFLAGS) -c src/main.c

token.o: src/token.c headers/token.h
	$(CC) $(CFLAGS) -c src/token.c

codebook.o: src/codebook.c headers/codebook.h
	$(CC) $(CFLAGS) -c src/codebook.c

cbtesting.o: headers/cbtesting.h
	touch cbtesting.c
	echo "#include \"headers/cbtesting.h\"" >> cbtesting.c
	gcc -c cbtesting.c
	rm cbtesting.c

libcbtesting.a: cbtesting.o
	ar rcs libcbtesting.a cbtesting.o

clean:
	rm *.o ./forge ./tests
