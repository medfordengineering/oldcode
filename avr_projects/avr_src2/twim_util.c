/* -------------------------------------------------------------
 Machine Science API

 (c) Copyright Machine Science, 2009.  All rights reserved

Version Date: Tue May  5 16:26:35 EDT 2009
-----------------------------------------------------------------
============================================================== */

#include <twim_util.h>

inline void ms_memset( void *data, register uint8_t c, register int len ) 
{
  register uint8_t *ptr = (uint8_t*)data;
  while( len-- > 0 ) {
    *ptr++ = c;
  }
}

inline uint8_t ms_strlen( const register char *text ) 
{
  register uint8_t len = 0;
  while( *text != '\0' ) {
    len++, text++;
  }
  
  return len;
}
