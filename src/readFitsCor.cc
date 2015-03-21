#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsCor.hh"

using namespace sli;

double readFitsCor::getCor(int id){
  double cor = fits.table( "FILTER" ).col( "COR" ).dvalue( id );  
  return cor;
}
