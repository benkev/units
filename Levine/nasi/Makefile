nasi:	nasi.lex.o nasi.tab.o
	g++ -g nasi.tab.o nasi.lex.o -lm  -o pc

nasi.lex.o: nasi.lex.c nasi.tab.h
	g++ -g -c nasi.lex.c

nasi.tab.o: nasi.tab.c nasi.lex.h 
	g++ -g -c nasi.tab.c

nasi.tab.c nasi.tab.h: nasi.y
	bison -vd nasi.y

nasi.lex.c nasi.lex.h: nasi.l
	flex -o nasi.lex.c nasi.l

clean:
	rm -f nasi.lex.o nasi.tab.o \
		  nasi.tab.c nasi.lex.c \
		  nasi.tab.h nasi.lex.h nasi.output pc
