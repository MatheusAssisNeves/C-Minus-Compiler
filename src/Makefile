CC = gcc

BIN = compiler.exe

OBJS = cmenos.tab.o lex.yy.o main.o util.o symtab.o analyze.o qgen.o quad.o cgen.o code.o binary.o

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN)

main.o: main.c globals.h util.h scan.h analyze.h qgen.h quad.h cgen.h code.h binary.h
	$(CC) -c main.c

util.o: util.c util.h globals.h
	$(CC) -c util.c

symtab.o: symtab.c symtab.h
	$(CC) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) -c analyze.c

lex.yy.o: cmenos.l scan.h util.h globals.h
	flex cmenos.l
	$(CC) -c lex.yy.c

cmenos.tab.o: cmenos.y quad.h 
	bison -d cmenos.y
	$(CC) -c cmenos.tab.c

quad.o: quad.c quad.h
	$(CC) -c quad.c

qgen.o: qgen.c qgen.h globals.h quad.h
	$(CC) -c qgen.c

code.o: code.c code.h
	$(CC) -c code.c

cgen.o: cgen.c cgen.h quad.h code.h
	$(CC) -c cgen.c

binary.o: binary.c binary.h quad.h code.h
	$(CC) -c binary.c

clean:
	-del /f $(BIN)
	-del /f cmenos.tab.c
	-del /f cmenos.tab.h
	-del /f lex.yy.c
	-del /f $(OBJS)
	#-del /f /s *.asm
	#-del /f /s *.bin
