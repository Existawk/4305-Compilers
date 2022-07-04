// Dalio, Brian A.
// dalioba
// 2021-04-19
//----------------------------------------------------------------
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "support.h"

#include "node.h"

#include "parser.tab.h"
#include "lex.yy.h"

//----------------------------------------------------------------
int main( int argc, char **argv )
{
  int exitStatus = EX_OK;

  //-- Construct the processing context --------------------------
  exitStatus = constructContext( argc, argv );
  if ( exitStatus != EX_OK ) goto cleanup;

  //-- Construct parse tree --------------------------------------
  FILE *fp = fopen( context.sourceFile, "r" );
  if ( fp == NULL ) {
    // Rats!  A file open-to-read error.
    printf( "// %s: Unable to open \"%s\" for read.  (%d) %s.\n",
      context.progName, context.sourceFile, errno, strerror( errno ) );
    exitStatus = EX_NOINPUT;
    goto cleanup;
  }

  printf( "// %s: Parsing \"%s\" ...\n",
    context.progName, context.sourceFile );

  // Read from this input file.
  yyrestart( fp, GET_SCANNER );

  // Parse the source file.
  Node *parse = NULL;
  int   status = yyparse( &parse, GET_SCANNER );

  // All done with the source file.
  fclose( fp ); fp = NULL;

  // Did the parse work?
  if ( status == 0 ) {
    printf( "// %s: ... parse succeeded.\n",
      context.progName );
  } else {
    printf( "// %s: ... parse failed with status %d.\n",
      context.progName, status );
    exitStatus = EX_PARSE_FAILURE;
    goto cleanup;
  }

  //-- Dump the parse tree ---------------------------------------
  fp = fopen( context.parseFile, "w" );
  if ( fp == NULL ) {
    // Rats!  A file open-to-write error.
    printf( "// %s: Unable to open \"%s\" for write.  (%d) %s.\n",
      context.progName, context.parseFile, errno, strerror( errno ) );
    exitStatus = EX_NOOUTPUT;
    goto cleanup;
  }

  printf( "// %s: Dumping parse tree to \"%s\" ...\n",
    context.progName, context.parseFile );

  dumpNodef( fp, parse );

  // All done with the parse file.
  fclose( fp ); fp = NULL;

  printf( "// %s: ... dump succeeded.\n",
    context.progName );

  //-- Perform semantic analysis ---------------------------------

  //-- Perform code generation -----------------------------------

  //-- Close / free all resources --------------------------------
cleanup:
  freeAllNodes();
  destructContext();

  //-- All done! -------------------------------------------------
  printf( "// %s: Exit status %d.\n",
    context.progName, exitStatus );

  return exitStatus;
}

//----------------------------------------------------------------
