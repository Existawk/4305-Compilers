/* Skinner, Parker R. */
/* prs1467         */
/* 2021-05-04      */

   /*--- DEFINITIONS -------------------------------------------*/
%{
#include <stdio.h>

#include "node.h"

#include "parser.tab.h"
#include "lex.yy.h"

void yyerror( YYLTYPE *, Node **, void *, const char * );

%}

%code requires {
  typedef void *yyscan_t;
}

%define api.pure full

%defines

%locations

%lex-param   { yyscan_t scanner }
%parse-param { Node **node }
%parse-param { yyscan_t scanner }

  /* Value stack type */
%union {
  Node *node;
  int intVal;
}

  /* Token names (and types, where necessary) */
%token tok_ASSIGN
%token tok_BREAK     tok_BY
%token tok_CONTINUE
%token tok_DOWNTO
%token tok_ELSE      tok_EQ
%token tok_FOR
%token tok_IF        tok_INTEGER
%token tok_NE
%token tok_REAL      tok_READ
%token tok_STRING
%token tok_TO
%token tok_WHILE     tok_WRITE

%token <node>   tok_INT_LIT
%token <node>   tok_REAL_LIT
%token <node>   tok_STR_LIT

%token <node>   tok_ID

  /* Nonterminal types */
%type <node>    block        itemStarList  itemPlusList  item
%type <node>    declaration  initOpt
%type <node>    ifStmt       elseOpt
%type <node>    readStmt
%type <node>    whileStmt
%type <node>    writeStmt
%type <node>    expr         exprList
%type <node>    breakStmt
%type <node>    continueStmt
%type <node>    forStmt
%type <node>    stepOpt
%type <intVal>  toDownto    

  /* Operator precedence / associativity */
%right     tok_ASSIGN
%nonassoc  tok_EQ tok_NE
%left      '-' '+'
%left      '/' '*'
%right     '^'
%right     NEGATE POSITE

%% /*--- RULES -------------------------------------------------*/

start
  : block     { *node = $1; }
  ;

  /*-- Block ---------------------------------------------------*/
block
  : '{' itemStarList '}'      { $$ = makeBlock( $2 ); }
  ;

itemStarList
  : epsilon                     { $$ = NULL; }
  | itemPlusList semicolonOpt   { $$ = $1; }
  ;

semicolonOpt
  : ';'
  | epsilon
  ;

itemPlusList
  : item                    { $$ = $1; }
  | itemPlusList ';' item   { $$ = nodeListAppend( $1, $3 ); }
  ;

item
  : declaration     { $$ = $1; }
  | ifStmt          { $$ = $1; }
  | readStmt        { $$ = $1; }
  | whileStmt       { $$ = $1; }
  | writeStmt       { $$ = $1; }
  | expr            { $$ = $1; }
  | breakStmt       { $$ = $1; }
  | continueStmt    { $$ = $1; }
  | forStmt         { $$ = $1; }
  ;

  /*-- Declarations --------------------------------------------*/
declaration
  : tok_INTEGER tok_ID initOpt  {
    if ( $3 == NULL ) { $3 = makeLitInteger( 0 ); }
    $$ = makeDeclaration( tok_INTEGER, $2, $3 );
  }
  | tok_REAL    tok_ID initOpt  {
    if ( $3 == NULL ) { $3 = makeLitReal( 0.0 ); }
    $$ = makeDeclaration( tok_REAL, $2, $3 );
  }
  | tok_STRING  tok_ID initOpt  {
    if ( $3 == NULL ) { $3 = makeLitString( "", 0 ); }
    $$ = makeDeclaration( tok_STRING, $2, $3 );
  }
  ;

initOpt
  : epsilon           { $$ = NULL; }
  | tok_ASSIGN expr   { $$ = $2; }
  ;

  /*-- If Statement --------------------------------------------*/
ifStmt
  : tok_IF expr block elseOpt   { $$ = makeIf( $2, $3, $4 ); }
  ;

elseOpt
  : epsilon         { $$ = makeBlock( NULL ); }
  | tok_ELSE block  { $$ = $2; }
  ;

  /*-- Read Statement ------------------------------------------*/
readStmt
  : tok_READ exprList     { $$ = makeRead( $2 ); }
  ;

  /*-- While Statement -----------------------------------------*/
whileStmt
  : tok_WHILE expr block  { $$ = makeWhile( $2, $3 ); }
  ;

  /*-- Write Statement -----------------------------------------*/
writeStmt
  : tok_WRITE exprList    { $$ = makeWrite( $2 ); }
  ;

  /*-- Break Statement -----------------------------------------*/
breakStmt
  : tok_BREAK      { $$ = makeBreak(); }
  ;

  /*-- Continue Statement --------------------------------------*/
continueStmt
  : tok_CONTINUE      { $$ = makeContinue(); }
  ;

  /*-- For Statement --------------------------------------*/
forStmt
  : tok_FOR tok_ID tok_ASSIGN expr toDownto expr stepOpt block { 
    if ($7 == NULL){$7 = makeLitInteger($5);}
    $$ = makeFor($2, $4, $5, $6, $7, $8); }
  ;

  /*-- stepOpt Statement --------------------------------------*/
stepOpt
  : epsilon         { $$ = NULL; }
  | tok_BY  expr    { $$ = $2; }
  ;

  /*-- toDownto Statement --------------------------------------*/
toDownto
  : tok_TO         { $$ = 1; }
  | tok_DOWNTO     { $$ = -1; }
  ;


  /*-- Expressions ---------------------------------------------*/
  /* Binary operators */
expr
  : expr '+' expr   { $$ = oprBinary( nBOP_ADD,      $1, $3 ); }
  | expr '/' expr   { $$ = oprBinary( nBOP_DIVIDE,   $1, $3 ); }
  | expr '^' expr   { $$ = oprBinary( nBOP_EXPONENT, $1, $3 ); }
  | expr '*' expr   { $$ = oprBinary( nBOP_MULTIPLY, $1, $3 ); }
  | expr '-' expr   { $$ = oprBinary( nBOP_SUBTRACT, $1, $3 ); }
  | expr tok_ASSIGN expr  { $$ = oprBinary( nBOP_ASSIGN,     $1, $3 ); }
  | expr tok_EQ     expr  { $$ = oprBinary( nBOP_EQUALS,     $1, $3 ); }
  | expr tok_NE     expr  { $$ = oprBinary( nBOP_NOT_EQUALS, $1, $3 ); }
  ;

  /* Unary operators */
expr
  : '-' expr  %prec NEGATE  { $$ = oprUnary( nUOP_NEGATE, $2 ); }
  | '+' expr  %prec POSITE  { $$ = oprUnary( nUOP_POSITE, $2 ); }
  ;

  /* Fundamentals */
expr
  : tok_ID          { $$ = $1; }
  | tok_INT_LIT     { $$ = $1; }
  | tok_REAL_LIT    { $$ = $1; }
  | tok_STR_LIT     { $$ = $1; }
  ;

  /* Nesting */
expr
  : '(' expr ')'    { $$ = $2; }
  ;

exprList
  : expr                  { $$ = $1; }
  | exprList ',' expr     { $$ = nodeListAppend( $1, $3 ); }
  ;

  /*-- The ever-popular Empty item -----------------------------*/
epsilon
  :
  ;

%% /*--- USER CODE ---------------------------------------------*/

// The yyerror routine is called by the parser engine whenever
//  it fails in parsing the token stream.
void yyerror( YYLTYPE *llocp, Node **_node, void *_scanner, const char *msg )
{
  /* Unused at present */ (void) _node; (void) _scanner;

  if ( llocp->first_line == llocp->last_line ) {
    if ( llocp->first_column == llocp->last_column ) {
      printf( "%d(%d): ",
        llocp->first_line, llocp->first_column );
    } else {
      printf( "%d(%d-%d): ",
        llocp->first_line,
        llocp->first_column, llocp->last_column );
    }
  } else {
    printf( "%d(%d)-%d(%d): ",
      llocp->first_line, llocp->first_column,
      llocp->last_line, llocp->last_column );
  }

  printf( "%s\n", msg );
}

/*--------------------------------------------------------------*/
