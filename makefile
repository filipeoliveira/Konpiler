all: trabalho entrada.de
	./trabalho < ex2.de > saida.cc
	cat saida.cc

lex.yy.c: trabalho.lex
	lex trabalho.lex

y.tab.c: trabalho.y
	bison trabalho.y

trabalho: lex.yy.c y.tab.c
	g++ -o trabalho y.tab.c -lfl
