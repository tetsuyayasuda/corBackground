#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsTime.hh"

using namespace sli;

double readFitsTime::getTime(int id){
  double time = fits.table( "FILTER" ).col( "TIME" ).dvalue( id );  
  return time;
}

double readFitsTime::getTimeStart(){
  double time = fits.table( "FILTER" ).col( "TIME" ).dvalue( 1 );  
  return time;
}

double readFitsTime::getTimeEnd(){
  long idEnd = fits.table( "FILTER" ).row_length() - 1;
  double time = fits.table( "FILTER" ).col( "TIME" ).dvalue( idEnd );  
  return time;
}

