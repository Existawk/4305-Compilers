// Dalio, Brian A.
// dalioba
// 2021-04-11
//----------------------------------------------------------------
#if !defined( __SUPPORT_H__ )
#define __SUPPORT_H__
//----------------------------------------------------------------
// Return codes (general) ...
#define OK      ( 0)
#define ERROR   (-1)

//----------------------------------------------------------------
// Exit codes ...
#include <sysexits.h>

#define EX_NOOUTPUT       ( 3)
#define EX_PARSE_FAILURE  ( 4)

#define EX_OUT_OF_MEMORY  (137)

//----------------------------------------------------------------
// Routines in support.c
extern char *getExt( char *fPath );
extern char *replaceExt( char *fPath, char *newExt );

//----------------------------------------------------------------
#endif
