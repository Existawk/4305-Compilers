// Dalio, Brian A.
// dalioba
// 2021-04-28
//----------------------------------------------------------------
#include <argp.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "support.h"

#include "node.h"

#include "parser.tab.h"
#include "lex.yy.h"

//----------------------------------------------------------------
// Establish context for processing.
//----------------------------------------------------------------

struct _context context;

//-- Command line option processing ------------------------------
// Argp is a useful set of routines for processing command line
//  arguments if you don't care to write your own specialized set.
//  https://www.gnu.org/software/libc/manual/html_node/Argp.html

const char *argp_program_version =
  "lang version " VERSION ", " __DATE__ " " __TIME__;

const char *argp_program_bug_address =
  "<  Hey, shouldn't you be _fixing_ the bugs yourself?  >";

static struct argp_option _argpOptions[] = {
  { "debug",   'd', 0, 0, "Produce debug output",   0 },
  { "lldebug", 'l', 0, 0, "Produce lldebug output", 0 },
  { "verbose", 'v', 0, 0, "Produce verbose output", 0 },
  { "yydebug", 'y', 0, 0, "Produce yydebug output", 0 },
  { 0 }
};

static error_t _argpParseOpt( int key, char *arg, struct argp_state *state )
{
  (void) arg; (void) state;

  switch ( key ) {
    case 'd' :
      context.debug++;
      break;

    case 'l' :
      setLLDebug( 1 );
      break;

    case 'v' :
      context.verbose++;
      break;

    case 'y' :
      yydebug = 1;
      break;

    default :
      return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp _argp = {
  _argpOptions, _argpParseOpt, "FILE", NULL, NULL, NULL, NULL
};

//-- Establish context -------------------------------------------
int constructContext( int argc, char **argv )
{
  //--------------------------------------
  // Enable thousand separators in printf().
  setlocale( LC_NUMERIC, "" );

  //--------------------------------------
  // Environment variable processing
  char *envyydebug = getenv( "YYDEBUG" );
  yydebug = ( envyydebug != NULL && envyydebug[0] == '1' );

  //--------------------------------------
  // Invoked name of program.
  context.progName = argv[0];

  //-- Initialize the Lexical Analyzer (the scanner) -------------
  if ( yylex_init( (yyscan_t *) &context.scanner ) ) {
    printf(
      "// %s: Unable to initialize the lexical analyzer.  Error is (%d) \"%s\".\n",
      context.progName, errno, strerror( errno ) );
    return EX_SOFTWARE;
  }

  //--------------------------------------
  // Command line option processing
  int argIndex;
  argp_parse( &_argp, argc, argv, 0, &argIndex, 0 );

  if ( argIndex >= argc ) {
    printf( "%s: No source file specified.\n", context.progName );
    exit( EX_USAGE );
  } else if ( argIndex != argc-1 ) {
    printf( "%s: Only one source file, please.\n", context.progName );
    exit( EX_USAGE );
  } else {
    context.sourceFile = argv[argIndex];
    context.parseFile  = replaceExt( context.sourceFile, "parse" );
    context.astFile    = replaceExt( context.sourceFile, "ast" );
    context.asmFile    = replaceExt( context.sourceFile, "asm" );
  }

  if ( context.debug ) {
    printf( "//-- Context ---------------------------\n" );
    printf( "// progName    : \"%s\"\n", context.progName );
    printf( "\n" );
    printf( "// debug       : %d\n",     context.debug );
    printf( "// lldebug     : %d\n",     getLLDebug() );
    printf( "// yydebug     : %d\n",     yydebug );
    printf( "// verbose     : %d\n",     context.verbose );
    printf( "\n" );
    printf( "// sourceFile  : \"%s\"\n", context.sourceFile );
    printf( "// parseFile   : \"%s\"\n", context.parseFile );
    printf( "// astFile     : \"%s\"\n", context.astFile );
    printf( "// asmFile     : \"%s\"\n", context.asmFile );
  }

  return EX_OK;
}

void destructContext()
{
  if ( context.parseFile ) free( context.parseFile );
  if ( context.astFile )   free( context.astFile );
  if ( context.asmFile )   free( context.asmFile );
  if ( GET_SCANNER )       yylex_destroy( GET_SCANNER );
}

int getLLDebug()
{
  return yyget_debug( GET_SCANNER );
}

void setLLDebug( int flag )
{
  yyset_debug( flag, GET_SCANNER );
}

//----------------------------------------------------------------
