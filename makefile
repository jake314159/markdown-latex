all: main

main: markdownlatex.o lexer.o parserstringops.o
	gcc bin/markdownlatex.o bin/lexer.o bin/parserstringops.o -std=c99 -Wall -o bin/markdownlatex

markdownlatex.o: src/markdownlatex.c
	gcc -std=c99 -Wall -c src/markdownlatex.c -o bin/markdownlatex.o

lexer.o: src/lexer.c
	gcc -std=c99 -Wall -c src/lexer.c -o bin/lexer.o

parserstringops.o: src/parserstringops.c
	gcc -std=c99 -Wall -c src/parserstringops.c -o bin/parserstringops.o
    
clean:
	rm -fr bin/*.o


#gcc src/markdownlatex.c src/lexer.c src/parserstringops.c -std=c99 -Wall -o bin/markdownlatex
