fb1-5: fb1-5.y fb1-5.l
	bison -d fb1-5.y
	flex -o fb1-5.lex.c fb1-5.l
	cc -o fb1-5 fb1-5.tab.c fb1-5.lex.c

clean:
	rm -f fb1-5.tab.c fb1-5.tab.h fb1-5.lex.c fb1-5



