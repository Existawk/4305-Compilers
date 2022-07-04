// Dalio, Brian A.
// dalioba
// 2021-04-19
//----------------------------------------------------------------
#if !defined( __CONTEXT_H__ )
#define __CONTEXT_H__
//----------------------------------------------------------------
#define VERSION "1.0"

struct _context {
  // Command line flags / arguments.
  int   debug;
  char *progName;
  char *sourceFile;
  int   verbose;

  // Names of intermediate files, constructed from the name of
  //  the source file.
  char *parseFile;
  char *astFile;
  char *asmFile;

  // Pointer to lexical scanner items.
  void *scanner;
};

extern struct _context context;

#define GET_SCANNER    ((yyscan_t) context.scanner )

//----------------------------------------------------------------
int  constructContext( int argc, char **argv );
void destructContext();
int  getLLDebug();
void setLLDebug( int flag );

//----------------------------------------------------------------
#endif
