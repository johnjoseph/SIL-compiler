all: compiler

lex.yy.c: lex.l
	lex lex.l

y.tab.c: code.h yacc.y
	yacc -d yacc.y

compiler: lex.yy.c y.tab.c
	gcc -o compiler lex.yy.c y.tab.c

clean:
	rm lex.yy.c y.tab.c	
