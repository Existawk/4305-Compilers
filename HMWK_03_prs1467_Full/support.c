// Dalio, Brian A.
// dalioba
// 2021-04-10
//----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "support.h"

//----------------------------------------------------------------
char *getExt( char *fPath )
{
  char *pstr;

  if ( !fPath || !*fPath ) {
    return NULL;
  }

  pstr = &fPath[ strlen( fPath ) - 1 ];

  while ( ( *pstr != '/' ) && ( *pstr != '.' ) && ( pstr > fPath ) ) {
    pstr--;
  }

  if ( *pstr != '.' ) {
    return NULL;
  }

  return pstr+1;
}

//----------------------------------------------------------------
char *replaceExt( char *fPath, char *newExt )
{
  char *ext = getExt( fPath );

  if ( ext == NULL ) {
    // Hmm -- There was no extension.  Well, just stick the new
    //  extension on the end (with a '.' prepended).
    int   oldLen = strlen( fPath );
    int   newLen = oldLen + 1 + strlen( newExt );
    char *newFPath  = (char *) malloc( newLen+1 );
    if ( newFPath == NULL ) {
      printf(
        "Unable to allocate replaceExt( '%s', '%s' ) path (%d).\n",
        fPath, newExt, newLen+1 );
      exit( EX_OUT_OF_MEMORY );
    }

    strcpy( newFPath, fPath );
    strcat( newFPath, "." );
    strcat( newFPath, newExt );

    return newFPath;

  } else {
    int   noExtLen  = strlen( fPath ) - strlen( ext );
    int   newExtLen = strlen( newExt );
    int   newLen    = noExtLen + newExtLen;
    char *newFPath  = (char *) malloc( newLen+1 );
    if ( newFPath == NULL ) {
      printf(
        "Unable to allocate replaceExt( '%s', '%s' ) path (%d).\n",
        fPath, newExt, newLen+1 );
      exit( EX_OUT_OF_MEMORY );
    }

    memcpy( newFPath, fPath, noExtLen );
    memcpy( &newFPath[noExtLen], newExt, newExtLen );

    newFPath[ newLen ] = '\0';

    return newFPath;
  }
}

//----------------------------------------------------------------
