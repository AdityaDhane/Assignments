solve:
	lex code.l
	bison -d code.y -o code.cc
	gcc -w -c lex.yy.c -o lex.yy.o
	g++ -w -o $@ lex.yy.o code.cc