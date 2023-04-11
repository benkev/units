purecalc:	purecalc.lex.o purecalc.tab.o purecalcfuncs.o
	cc -g purecalc.tab.o purecalc.lex.o purecalcfuncs.o -lm  -o pc

purecalc.lex.o: purecalc.lex.c purecalc.tab.h purecalc.h

purecalc.tab.o: purecalc.tab.c purecalc.lex.h purecalc.h

purecalc.lex.c purecalc.lex.h: purecalc.l
	flex -o purecalc.lex.c purecalc.l

purecalc.tab.c purecalc.tab.h: purecalc.y
	bison -vd purecalc.y
 

clean:
	rm -f purecalc.lex.o purecalc.tab.o purecalcfuncs.o \
		  purecalc.tab.c purecalc.lex.c \
		  purecalc.tab.h purecalc.lex.h purecalc.output pc
