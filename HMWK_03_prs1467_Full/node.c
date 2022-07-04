// Skinner, Parker R.
// prs1467
// 2021-05-04
//----------------------------------------------------------------
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "support.h"
#include "tokenNames.h"

//----------------------------------------------------------------
static void _dumpStringReadable( FILE *fp, char *value );

//----------------------------------------------------------------
// These strings ABSOLUTELY have to be in the same order as the
//  entries in the enum _NodeKind defined in node.h.
static char *_nodeKindStr[] = {
  "BLOCK", "BREAK", "CONTINUE", "DECLARATION", "FOR", "IF",
  "READ", "WHILE", "WRITE",

  "ADD", "ASSIGN", "DIVIDE", "EQUALS", "EXPONENT", "MULTIPLY",
  "NOT_EQUALS", "SUBTRACT",

  "ID",

  "INTEGER", "REAL", "STRING",

  "NEGATE", "POSITE"
};

//----------------------------------------------------------------
// We keep a linked list of every Node node that gets allocated
//   so they are easy to clean up later.
static Node *_nodeList;

// Allocate a node
Node *allocNode( NodeKind kind )
{
  // Get a zeroed-out node structure.
  Node *n = (Node *) calloc( 1, sizeof( Node ) );

  // Oops, couldn't allocate!
  if ( n == NULL ) {
    printf( "Unable to allocate a Node structure?\n" );
    exit( EX_OUT_OF_MEMORY );
  }

  // Make the newly allocated node whatever kind was requested.
  //  (Remember, all other fields will be zero since we used
  //   calloc().)
  n->kind = kind;

  // Stick this newly created node on the list of all node
  //  structs.
  n->next   = _nodeList;
  _nodeList = n;

  // Success!  Return the literal we just created.
  return n;
}

// Free all Node nodes that have been allocated so far.
void freeAllNodes()
{
  Node *n    = _nodeList;
  Node *next = NULL;

  // As long as there's a node struct on the node struct list,
  //  free it.  We go down the list freeing them one after
  //  another, remembering the pointer to the next one as we go.
  while ( n ) {
    next = n->next;

    if ( n->kind == nID || n->kind == nLIT_STRING ) free( n->strval );
    free( n );

    n = next;
  }

  // List is empty now ...
  _nodeList = NULL;
}

//----------------------------------------------------------------
// Dump the given Node node (hierarchically).
static void _dumpNodeInternal( Node *n, int indent, FILE *fp )
{
  switch ( n->kind ) {
    //--------------------------------------
    // Statements
    case nBLOCK :
      fprintf( fp, "%*c(BLOCK\n",
        indent, ' ' );

      for ( Node *item = n->left; item; item = item->listNext ) {
        _dumpNodeInternal( item, indent+1, fp );
      }

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;
    case nBREAKSTMT :
      fprintf( fp, "%*c(BREAK)\n",
        indent, ' ' );

      break;
    case nCONTINUESTMT:
      fprintf( fp, "%*c(CONTINUE)\n",
        indent, ' ' );
      
      break;

      case nFORSTMT :
      fprintf( fp, "%*c(FOR \"%s\" %d\n",
        indent, ' ' , n->ForId->strval, n->ToDownto);

      _dumpNodeInternal( n->startexpr, indent+1, fp );
      _dumpNodeInternal( n->stopexpr, indent+1, fp );
      _dumpNodeInternal( n->stepexpr, indent+1, fp );
      _dumpNodeInternal( n->bodynode, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    case nDECLARATION :
      fprintf( fp, "%*c(DECLARATION %s \"%s\"\n",
        indent, ' ', tokenToName( n->type ), n->id->strval );

      _dumpNodeInternal( n->init, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    case nIFSTMT :
      fprintf( fp, "%*c(IF\n",
        indent, ' ' );

      _dumpNodeInternal( n->ifTest,      indent+1, fp );
      _dumpNodeInternal( n->ifThenBlock, indent+1, fp );
      _dumpNodeInternal( n->ifElseBlock, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    case nREADSTMT :
      fprintf( fp, "%*c(READ\n",
        indent, ' ' );

      for ( Node *item = n->exprList; item; item = item->listNext ) {
        _dumpNodeInternal( item, indent+1, fp );
      }

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    case nWHILESTMT :
      fprintf( fp, "%*c(WHILE\n",
        indent, ' ' );

      _dumpNodeInternal( n->whileTest, indent+1, fp );
      _dumpNodeInternal( n->whileBody, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    case nWRITESTMT :
      fprintf( fp, "%*c(WRITE\n",
        indent, ' ' );

      for ( Node *item = n->exprList; item; item = item->listNext ) {
        _dumpNodeInternal( item, indent+1, fp );
      }

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    //--------------------------------------
    // Binary operators
    case nBOP_ADD :
    case nBOP_ASSIGN :
    case nBOP_DIVIDE :
    case nBOP_EQUALS :
    case nBOP_EXPONENT :
    case nBOP_MULTIPLY :
    case nBOP_NOT_EQUALS :
    case nBOP_SUBTRACT :
      fprintf( fp, "%*c(BOP %s\n",
        indent, ' ', _nodeKindStr[ n->kind ] );

      _dumpNodeInternal( n->left,  indent+1, fp );
      _dumpNodeInternal( n->right, indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    //--------------------------------------
    // Unary operators
    case nUOP_NEGATE :
    case nUOP_POSITE :
      fprintf( fp, "%*c(UOP %s\n",
        indent, ' ', _nodeKindStr[ n->kind ] );

      _dumpNodeInternal( n->left,  indent+1, fp );

      fprintf( fp, "%*c)\n",
        indent, ' ' );

      break;

    //--------------------------------------
    // Name
    case nID :
      fprintf( fp, "%*c(ID \"%s\")\n",
        indent, ' ', n->strval );

      break;

    //--------------------------------------
    // Literals
    case nLIT_INTEGER :
      fprintf( fp, "%*c(LITERAL %s %d)\n",
        indent, ' ', _nodeKindStr[ n->kind ], n->litInteger );

      break;

    case nLIT_REAL :
      fprintf( fp, "%*c(LITERAL %s %.16e)\n",
        indent, ' ', _nodeKindStr[ n->kind ], n->litReal );

      break;

    case nLIT_STRING :
      fprintf( fp, "%*c(LITERAL %s \"",
        indent, ' ', _nodeKindStr[ n->kind ] );

      _dumpStringReadable( fp, n->strval );

      fprintf( fp, "\")\n" );

      break;

    //--------------------------------------
    // Don't you hate it when something like this happens?
    default :
      fprintf( stderr, "Unknown node kind %d.\n", n->kind );

      break;
  }
}

void dumpNode( Node *n )
{
  _dumpNodeInternal( n, 1, stdout );
}

void dumpNodef( FILE *fp, Node *n )
{
  _dumpNodeInternal( n, 1, fp );
}

static void _dumpStringReadable( FILE *fp, char *value )
{
  for ( ; *value; value++ ) {
    switch ( *value ) {
      case '"'  :
      case '\\' :
        fprintf( fp, "\\%c", *value );
        break;

      default :
        if ( isprint( *value ) ) {
          fputc( *value, fp );
        } else {
          fprintf( fp, "\\%o", 0xFF & *value );
        }
        break;
    }
  }
}

//----------------------------------------------------------------
Node *makeBlock( Node *n )
{
  Node *b = allocNode( nBLOCK );

  b->left = n;

  return b;
}

Node *makeDeclaration( int type, Node *id, Node *initExpr )
{
  Node *n = allocNode( nDECLARATION );

  n->type = type;
  n->id   = id;
  n->init = initExpr;

  return n;
}

Node *makeId( char *name )
{
  Node *n = allocNode( nID );

  n->strval = strdup( name );

  return n;
}

Node *makeIf( Node *test, Node *thenBlock, Node *elseBlock )
{
  Node *n = allocNode( nIFSTMT );

  n->ifTest      = test;
  n->ifThenBlock = thenBlock;
  n->ifElseBlock = elseBlock;

  return n;
}

Node *makeLitInteger( int value )
{
  Node *n = allocNode( nLIT_INTEGER );

  n->litInteger = value;

  return n;
}

Node *makeLitReal( double value )
{
  Node *n = allocNode( nLIT_REAL );

  n->litReal = value;

  return n;
}

Node *makeLitString( char *value, int trim )
{
  Node *n        = allocNode( nLIT_STRING );
  int   len      = strlen( value );
  int   allocLen = trim ? len-1 : len+1;

  n->strval = (char *) calloc( allocLen, sizeof( char ) );
  if ( n->strval == NULL ) {
    printf( "Unable to allocate a string space of %d char%s?\n",
      allocLen, allocLen == 1 ? "" : "s" );
    exit( EX_OUT_OF_MEMORY );
  }

  int start = trim ? 1 : 0;
  int last  = trim ? len-2 : len-1;
  int loc   = 0;

  for ( int i = start; i <= last; i++, loc++ ) {
    if ( value[i] == '\\' ) {
      i++;
      switch ( value[i] ) {
        case '\'' :
        case '"'  :
        case '?'  :
        case '\\' :
          n->strval[loc] = value[i];
          break;

        case 'a'  : n->strval[loc] = '\a'; break;
        case 'b'  : n->strval[loc] = '\b'; break;
        case 'f'  : n->strval[loc] = '\f'; break;
        case 'n'  : n->strval[loc] = '\n'; break;
        case 'r'  : n->strval[loc] = '\r'; break;
        case 't'  : n->strval[loc] = '\t'; break;
        case 'v'  : n->strval[loc] = '\v'; break;

        case '0' : case '1' : case '2' : case '3' :
        case '4' : case '5' : case '6' : case '7' : {
          char *endPtr;

          n->strval[loc] = (int) strtol( value+i, &endPtr, 8 );
          i = ( (int) (endPtr - value) ) - 1;
          break;
        }

        case 'x': {
          char *endPtr;

          n->strval[loc] = (int) strtol( value+i+1, &endPtr, 16 );
          i = ( (int) (endPtr - value) ) - 1;
          break;
        }

        default :
          printf( "Unknown escape character %c (%d) seen in string literal?\n",
            value[i], value[i] );
          n->strval[loc] = '?';
          break;
      }
    } else {
      n->strval[loc] = value[i];
    }
  }

  return n;
}

Node *makeRead( Node *list )
{
  Node *n = allocNode( nREADSTMT );

  n->exprList = list;

  return n;
}

Node *makeWhile( Node *test, Node *body )
{
  Node *n = allocNode( nWHILESTMT );

  n->whileTest = test;
  n->whileBody = body;

  return n;
}

Node *makeWrite( Node *list )
{
  Node *n = allocNode( nWRITESTMT );

  n->exprList = list;

  return n;
}

Node *makeBreak()
{
  Node *n = allocNode( nBREAKSTMT );

  return n;
}

Node *makeContinue()
{
  Node *n = allocNode( nCONTINUESTMT );

  return n;
}

Node *makeFor( Node *id,Node *start,int todownto,Node *stop,Node *step, Node *body )
{
  Node *n = allocNode( nFORSTMT );

  n->ForId = id;
  n->startexpr = start;
  n->ToDownto = todownto;
  n->stopexpr = stop;
  n->stepexpr = step;
  n->bodynode = body;

  return n;
}


//----------------------------------------------------------------
// Append a node to the given list of nodes.
Node *nodeListAppend( Node *list, Node *item )
{
  Node *result = list;

  while ( list->listNext ) {
    list = list->listNext;
  }

  list->listNext = item;

  return result;
}

//----------------------------------------------------------------
// Build a binary operator Node node with the given left and
//   right operands.
//   Return NULL if the allocation fails.
Node *oprBinary( NodeKind kind, Node *left, Node *right )
{
  Node *n = allocNode( kind );

  n->left  = left;
  n->right = right;

  return n;
}

// Build a unary operator Node node with the given operand.
//   Return NULL if the allocation fails.
Node *oprUnary( NodeKind kind, Node *operand )
{
  Node *n = allocNode( kind );

  n->left  = operand;

  return n;
}

//----------------------------------------------------------------
