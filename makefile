lexical_analyser: main.o lexer.o
	gcc main.o lexer.o -o lexical_analyser

main.o: main.c lexer.h
	gcc -c main.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

clean:
	rm -f *.o lexical_analyser
