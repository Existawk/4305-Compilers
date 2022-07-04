# Dalio, Brian A.
# dalioba
# 2021-04-19

FILES   = context.c lex.yy.c main.c node.c parser.tab.c support.c tokenNames.c
CC      = gcc
CFLAGS  = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan
DEFINES = -DYYDEBUG
LIBS    =

lang : $(FILES)
	$(CC) $(DEFINES) $(CFLAGS) $(FILES) -o lang $(LIBS)

lex.yy.c : lexer.l
	flex lexer.l

parser.tab.c : parser.y lex.yy.c
	bison --report=all --xml parser.y
	xsltproc /usr/share/bison/xslt/xml2xhtml.xsl parser.xml > parser.html
	rm -f parser.xml

clean:
	rm -f *.o lex.yy.[ch] parser.tab.[ch] parser.output parser.xml parser.html lang *.parse *.ast *.asm
