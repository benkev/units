ru:	read_units_a.y read_units_a.l
	bison -d read_units_a.y
	flex -o read_units_a.lex.c read_units_a.l
	gcc read_units_a.tab.c read_units_a.lex.c -lm -o ru

clean:
	rm -f read_units_a.tab.h read_units_a.tab.c read_units_a.lex.c rua
