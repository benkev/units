current_time := $(shell date "+%Y.%m.%d-%H.%M.%S")
trace_file := bison_trace_$(current_time).txt

ru:	read_units.y read_units.l read_units.c
	strace -o ${trace_file} -f -e trace=file bison -dt read_units.y
	flex -o read_units.lex.c read_units.l
	gcc -g read_units.c read_units.tab.c read_units.lex.c read_units_funcs.c \
		-lm -o ru

clean:
	rm -f read_units.tab.h read_units.tab.c read_units.lex.h read_units.lex.c ru
