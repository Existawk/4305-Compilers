// Dalio, Brian A.
// dalioba
// 2021-04-26
//----------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "context.h"
#include "node.h"
#include "tokenNames.h"

#include "parser.tab.h"
//----------------------------------------------------------
typedef struct {
  char *name;
  int   token;
} TokenName;

//----------------------------------------------------------
static TokenName keywords[] = {
  // Keywords
  { "break",     tok_BREAK },
  { "by",        tok_BY },
  { "continue",  tok_CONTINUE },
  { "downto",    tok_DOWNTO },
  { "else",      tok_ELSE },
  { "for",       tok_FOR },
  { "if",        tok_IF },
  { "read",      tok_READ },
  { "to",        tok_TO },
  { "write",     tok_WRITE },
  { "while",     tok_WHILE },

  // "Type name" keywords
  { "integer",   tok_INTEGER },
  { "real",      tok_REAL },
  { "string",    tok_STRING },

  // Operators
  { "<-",        tok_ASSIGN },
  { "==",        tok_EQ },
  { "<>",        tok_NE },

  { "+",         '+' },
  { "-",         '-' },
  { "*",         '*' },
  { "/",         '/' },
  { "%",         '%' },
  { "^",         '^' },
  { "!",         '!' },
  { "~",         '~' },
  { "?",         '?' },

  // "Punctuation"
  { ",",         ',' },
  { ";",         ';' },
  { "(",         '(' },
  { ")",         ')' },

  // "Categories"
  { "<REALLIT>", tok_REAL_LIT },
  { "<ID>",      tok_ID       },
  { "<INTLIT>",  tok_INT_LIT  },
  { "<STRLIT>",  tok_STR_LIT  },
};

#define NUM_KEYWORDS ( sizeof(keywords) / sizeof(TokenName) )

//----------------------------------------------------------
int isKeyword( const char *str )
{
  for ( size_t i=0; i < NUM_KEYWORDS; i++ ) {
    if ( strcmp( str, keywords[i].name ) == 0 ) {
      if ( getLLDebug() ) {
        printf( "'%s' recognized as token %d.\n", str, keywords[i].token );
      }

      return keywords[i].token;
    }
  }

  if ( getLLDebug() ) {
    printf( "'%s' unrecognized as keyword.\n", str );
  }

  return 0;
}

char *tokenToName( int token )
{
  for ( size_t i=0; i < NUM_KEYWORDS; i++ ) {
    if ( keywords[i].token == token ) {
      if ( getLLDebug() ) {
        printf( "%d recognized as token '%s'.\n", token, keywords[i].name );
      }

      return keywords[i].name;
    }
  }

  if ( getLLDebug() ) {
    printf( "%d unrecognized as token.\n", token );
  }

  return NULL;
}

//----------------------------------------------------------
