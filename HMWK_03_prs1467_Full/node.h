// Skinner, Parker R.
// prs1467
// 2021-05-04
//----------------------------------------------------------------
#if !defined( __NODE_H__ )
#define __NODE_H__

#include <stdio.h>

//----------------------------------------------------------------
// If you change this enum, you ABSOLUTELY have to match matching
//  changes to the declaration of _nodeKindStr in node.c.
typedef enum _NodeKind
{
  //-- Statement node kinds --------------------------------------
  nBLOCK,         // a list of decls and/or stmts
  nBREAKSTMT,     // <no other fields required>
  nCONTINUESTMT,  // <no other fields required>
  nDECLARATION,   // a type, an id, and an optional initializer expr
  nFORSTMT,       // index id, start expr, to/downto flag, stop expr, step expr, body
  nIFSTMT,        // an expr to test, a then block, an optional else block
  nREADSTMT,      // expr list
  nWHILESTMT,     // an expr to test and a block to run
  nWRITESTMT,     // expr list

  //-- Expression node kinds -------------------------------------
  nBOP_ADD,
  nBOP_ASSIGN,
  nBOP_DIVIDE,
  nBOP_EQUALS,
  nBOP_EXPONENT,
  nBOP_MULTIPLY,
  nBOP_NOT_EQUALS,
  nBOP_SUBTRACT,

  nID,

  nLIT_INTEGER,
  nLIT_REAL,
  nLIT_STRING,

  nUOP_NEGATE,
  nUOP_POSITE,
} NodeKind;

typedef struct _Node
{
  struct _Node *next;       // For the "all nodes" list
  struct _Node *listNext;   // For user-level list

  NodeKind kind;

  //-- Declaration -----------------------------------------------
  int     type;
  struct _Node *id;
  struct _Node *init;

  //-- For statement ---------------------------------------------
  // ... put field declarations here for
  //  index ID node
  //  start expr node
  //  TO / DOWNTO flag
  //  stop expr node
  //  step expr node
  //  body node
  struct _Node *ForId;
  struct _Node *startexpr;
  int ToDownto;
  struct _Node *stopexpr;
  struct _Node *stepexpr;
  struct _Node *bodynode;

  //-- If statement ----------------------------------------------
  struct _Node *ifTest;
  struct _Node *ifThenBlock;
  struct _Node *ifElseBlock;

  //-- Read, Write statements ------------------------------------
  struct _Node *exprList;

  //-- While statement -------------------------------------------
  struct _Node *whileTest;
  struct _Node *whileBody;

  //-- Literals, ID ----------------------------------------------
  int     litInteger;
  double  litReal;
  char   *strval;

  //-- Unary / Binary operators ----------------------------------
  struct _Node *left;
  struct _Node *right;
} Node;

#define ENSURE_REAL(n) ( (n)->kind == nLIT_REAL ? (n)->litReal : (double) (n)->litInteger )

//----------------------------------------------------------------
Node *allocNode( NodeKind kind );
void  dumpNode( Node *n );
void  dumpNodef( FILE *fp, Node *n );

Node *evalNode( Node *n );
void  freeAllNodes();

Node *makeBlock( Node *n );
Node *makeDeclaration( int kind, Node *id, Node *initExpr );
Node *makeId( char *id );
Node *makeIf( Node *test, Node *thenBlock, Node *elseBlock );
Node *makeLitInteger( int value );
Node *makeLitReal( double value );
Node *makeLitString( char *value, int trim );
Node *makeRead( Node *list );
Node *makeWhile( Node *test, Node *body );
Node *makeWrite( Node *list );
Node *makeBreak();
Node *makeContinue();
Node *makeFor( Node *id,Node *start,int todownto,Node *stop,Node *step, Node *body );
Node *nodeListAppend( Node *list, Node *item );

Node *oprBinary( NodeKind kind, Node *left, Node *right );
Node *oprUnary( NodeKind kind, Node *operand );

//----------------------------------------------------------------
#endif
