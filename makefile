
USR_BIN = /usr/bin

all: main
test: tester

main: markdownlatex.o lexer.o parserstringops.o tableProcessor.o
	$(CC) bin/markdownlatex.o bin/lexer.o bin/parserstringops.o bin/tableProcessor.o -std=c99 -Wall -o bin/markdownlatex

markdownlatex.o: src/markdownlatex.c
	$(CC) -std=c99 -Wall -c src/markdownlatex.c -o bin/markdownlatex.o

lexer.o: src/lexer.c
	$(CC) -std=c99 -Wall -c src/lexer.c -o bin/lexer.o

parserstringops.o: src/parserstringops.c
	$(CC) -std=c99 -Wall -c src/parserstringops.c -o bin/parserstringops.o

tableProcessor.o: src/tableProcessor.c
	$(CC) -std=c99 -Wall -c src/tableProcessor.c -o bin/tableProcessor.o

tester.o: tests/tester.c
	$(CC) -std=c99 -Wall -c tests/tester.c -o testbin/tester.o
 
clean:
	rm -fr testbin/*.o
	rm -fr bin/*.o

install:
	cp bin/markdownlatex $(USR_BIN)/markdownlatex

tester: main tester.o
	$(CC) testbin/tester.o bin/parserstringops.o bin/lexer.o -o testbin/tester
	./testbin/tester


#gcc src/markdownlatex.c src/lexer.c src/parserstringops.c -std=c99 -Wall -o bin/markdownlatex
