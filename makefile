
SHELL = /bin/sh

#Users bin (used when installing)
USR_BIN = /usr/bin

# Name of the generated binary
FILE_OUT = markdownlatex

BIN = bin
SRC = src
srcdir = .

CC = gcc
CCFLAGS = -std=c99 -Wall

all: main
check: tester

main: bindir markdownlatex.o lexer.o parserstringops.o tableProcessor.o
	$(CC) $(srcdir)/$(BIN)/markdownlatex.o $(srcdir)/$(BIN)/lexer.o $(srcdir)/$(BIN)/parserstringops.o $(srcdir)/$(BIN)/tableProcessor.o $(CCFLAGS) -o $(srcdir)/$(BIN)/$(FILE_OUT)

markdownlatex.o: src/markdownlatex.c bindir $(srcdir)/$(SRC)/lexer.h $(srcdir)/$(SRC)/parserstringops.h $(srcdir)/$(SRC)/tableProcessor.h $(srcdir)/$(SRC)/stdvals.h $(srcdir)/$(SRC)/versionData.h $(srcdir)/$(SRC)/markdownlatex.h
	$(CC) $(CCFLAGS) -c $(srcdir)/$(SRC)/markdownlatex.c -o $(srcdir)/$(BIN)/markdownlatex.o

lexer.o: $(srcdir)/$(SRC)/lexer.c bindir $(srcdir)/$(SRC)/lexer.h
	$(CC) $(CCFLAGS) -c $(srcdir)/$(SRC)/lexer.c -o $(srcdir)/$(BIN)/lexer.o

parserstringops.o: $(srcdir)/$(SRC)/parserstringops.c bindir $(srcdir)/$(SRC)/parserstringops.h $(srcdir)/$(SRC)/stdvals.h
	$(CC) $(CCFLAGS) -c $(srcdir)/$(SRC)/parserstringops.c -o $(srcdir)/$(BIN)/parserstringops.o

tableProcessor.o: $(srcdir)/$(SRC)/tableProcessor.c bindir $(srcdir)/$(SRC)/lexer.h $(srcdir)/$(SRC)/parserstringops.h $(srcdir)/$(SRC)/stdvals.h
	$(CC) $(CCFLAGS) -c $(srcdir)/$(SRC)/tableProcessor.c -o $(srcdir)/$(BIN)/tableProcessor.o

bindir:
	[ -f $(BIN) ] || mkdir -p $(BIN)

testbindir:
	mkdir -p testbin

tester.o: testbindir tests/tester.c
	$(CC) $(CCFLAGS) -c tests/tester.c -o testbin/tester.o

clean:
	rm -fr testbin/*.o
	rm -fr $(srcdir)/$(BIN)/*.o

install: $(srcdir)/$(BIN)/markdownlatex
	cp $(srcdir)/$(BIN)/markdownlatex $(USR_BIN)/markdownlatex

uninstall: $(USR_BIN)/markdownlatex
	rm $(USR_BIN)/markdownlatex

tester: main tester.o testbindir
	$(CC) testbin/tester.o $(srcdir)/$(BIN)/parserstringops.o $(srcdir)/$(BIN)/lexer.o -o testbin/tester
	./testbin/tester

dist:
	echo "Not yet implemented"

#gcc src/markdownlatex.c src/lexer.c src/parserstringops.c $(CCFLAGS) -o bin/markdownlatex
