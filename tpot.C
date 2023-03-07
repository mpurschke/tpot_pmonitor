#include "tpot.h"
R__LOAD_LIBRARY(libtpot.so)

void tpot(const char * filename)
{
  if ( filename != NULL)
    {
      pfileopen(filename);
    }
}
