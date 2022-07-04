/* Skinner, Parker R. */
/* prs1467         */
/* 2021-05-04      */

  //---- DEFINITIONS  --------------------------------------------

%option debug

/* Ensure the lexer is reentrant -- avoid the original mess of
    global variables for everything. */
%option reentrant

/* We're a lexer for a bison-generated parser, so adjust our
    functional interface to match. */
%option bison-bridge

/* Establish the connection between bison and flex for tracking
    locations of tokens.  (Even with this, the locations are NOT
    tracked automatically.  We have to do that ourself.  See
    below for more details. */
%option bison-locations

/* Create the lexer exports header file with this name. */
%option header-file="lex.yy.h"

/* Our input is not from an interactive source. */
%option never-interactive
/* Don't generate the default rule. */
%option nodefault
/* We don't directly read the input. */
%option noinput
/* Don't include <unistd.h>. */
%option nounistd
/* We don't 'unput' a character back. */
%option nounput
/* Don't call yywrap() on EOF. */
%option noyywrap

/* These two options are doubled to get more detailed reports. */
/* Write performance report to stderr. */
%option perf-report perf-report
/* Write statistics summary to stderr. */
%option verbose verbose

/* Generate warning messages for goofs. */
%option warn

/* Maintain current line number in yylineno. */
%option yylineno

%{
#include <stdio.h>

#include "context.h"
#include "node.h"
#include "parser.tab.h"
#include "tokenNames.h"

void printLocation( YYLTYPE *loc, FILE *fp );
static void _updateYYLLOC( char *txt, YYLTYPE *loc, int lineno );

/* YY_USER_ACTION is automatically inserted at the beginning of
    every action routine.  All we use it for is to help us do the
    tracking of token line and column locations. */
#define YY_USER_ACTION _updateYYLLOC( yytext, yylloc, yylineno );

%}

/* Defined names ... */
BIT           [01]
DIGIT         [0-9]
ESCAPE_CHAR   \\(['"?\\abfnrtv]|([0-3]?[0-7]{1,2})|(x{HEXIT}?{HEXIT}))
EXPONENT      [eE][-+]?{DIGIT}+
HEXIT         [a-fA-F0-9]
OCTIT         [0-7]

%% //---- RULES --------------------------------------------------

  /*-- Integer literals ----------------------------------------*/

0b{BIT}+  {
  // A string of binary digits -- an integer literal.
  yylval->node = makeLitInteger( (int) strtol( yytext+2, NULL, 2 ) );
  return tok_INT_LIT;
}

0o{OCTIT}+  {
  // A string of octal digits -- an integer literal.
  yylval->node = makeLitInteger( (int) strtol( yytext+2, NULL, 8 ) );
  return tok_INT_LIT;
}

0x{HEXIT}+  {
  // A string of hexadecimal digits -- an integer literal.
  yylval->node = makeLitInteger( (int) strtol( yytext+2, NULL, 16 ) );
  return tok_INT_LIT;
}

{DIGIT}+  {
  // A string of decimal digits -- an integer literal.
  yylval->node = makeLitInteger( (int) strtol( yytext, NULL, 10 ) );
  return tok_INT_LIT;
}

  /*-- Real literals -------------------------------------------*/

(({DIGIT}*(({DIGIT}\.)|(\.{DIGIT})){DIGIT}*){EXPONENT}?) {
  // The kind of real literal that has a decimal point in it
  //  somewhere;  either at the front, the back, or somewhere in
  //  the middle.  The exponent is optional since the DP indicates
  //  it's a real literal.
  yylval->node = makeLitReal( atof( yytext ) );
  return tok_REAL_LIT;
}

({DIGIT}+{EXPONENT})  {
  // The kind of real literal that does NOT have a DP.  For this
  //  style, the exponent is required.
  yylval->node = makeLitReal( atof( yytext ) );
  return tok_REAL_LIT;
}

  /*-- String literal ------------------------------------------*/
["]([^\n\\"]|{ESCAPE_CHAR})*["]     {
  // A string literal -- zero or more "regular" or "escape"
  //  characters, all between " marks.  A "regular" character is
  //  anything but a newline, backslash, or ".  The "escape"
  //  characters are shown in the Defined Names section above.
  yylval->node = makeLitString( yytext, 1 );
  return tok_STR_LIT;
}

  /*-- Identifier ----------------------------------------------*/

[_a-zA-Z][_a-zA-Z0-9]*  {
  // An ID or a keyword.

  // Is it a keyword?
  int kw = isKeyword( yytext );
  if ( kw ) {
    // Yep.
    return kw;
  }

  // Not a keyword, so it must be an ID.
  yylval->node = makeId( yytext );

  return tok_ID;
}

  /*-- Single character tokens ---------------------------------*/

[-=+/*^(){};,]     { return yytext[0]; }

  /*-- Multi-character tokens ----------------------------------*/

"<-"  { return tok_ASSIGN; }
"=="  { return tok_EQ; }
"<>"  { return tok_NE; }

  /*-- Ignorable whitespace ------------------------------------*/

[ \f\n\r\t\v]+  {}

  /*-- Comment -------------------------------------------------*/

"//".*          {}

  /*-- Illegal character ---------------------------------------*/

.           {
  // Ignore the illegal character, but print a message.
  printLocation( yylloc, stdout );

  printf( ":  Illegal character '%s' ignored.\n", yytext );
}

%% //---- USER CODE ----------------------------------------------

void printLocation( YYLTYPE *loc, FILE *fp )
{
  if ( loc->first_line == loc->last_line ) {
    if ( loc->first_column == loc->last_column ) {
      fprintf( fp, "%d(%d)",
        loc->first_line, loc->first_column );
    } else {
      fprintf( fp, "%d(%d-%d)",
        loc->first_line,
        loc->first_column, loc->last_column );
    }
  } else {
    fprintf( fp, "%d(%d)-%d(%d)",
      loc->first_line, loc->first_column,
      loc->last_line, loc->last_column );
  }
}

/*----------------------------------------------------------------
  All the poop we need to track the location of a token:
    which line it's on, which column it starts in, and which
    column it ends in. */
static int yycolnoBegin;
static int yycolnoEnd;

void resetColumnPosition()
{
  yycolnoBegin = yycolnoEnd = 0;
}

static void _updateYYLLOC( char *txt, YYLTYPE *loc, int lineno )
{
  // The first line of this token has to be the last line of the
  //  previous token.
  loc->first_line = loc->last_line;

  // The last line of this token is whatever line flex thinks we
  //  are on now.
  loc->last_line  = lineno;

  // The first column of this token has to be one past the last
  //  column of the previous token.
  yycolnoBegin = yycolnoEnd + 1;

  // We count the characters this token matched to determine its
  //  last column.  If the token includes a newline or a carriage
  //  return, we start over again at the left margin.
  for ( int i=0; txt[i] != '\0'; i++ ) {
    if ( txt[i] == '\n' || txt[i] == '\r' ) {
      yycolnoEnd = 0;
    } else {
      yycolnoEnd++;
    }
  }

  // Now that we know the begin / end column of the token, place
  //  this info in the yylloc location so flex / bison can see it.
  loc->first_column = yycolnoBegin;
  loc->last_column  = yycolnoEnd;

  if ( getLLDebug() ) {
    // Useful info when debugging the scanner, but very noisy for
    //  any production use.
    printf( "[ Scanner match " );
    printLocation( loc, stdout );
    printf( ": >%s< ]\n", txt );
  }
}

//----------------------------------------------------------------
