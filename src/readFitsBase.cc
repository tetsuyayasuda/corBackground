#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsBase.hh"

using namespace sli;

readFitsBase::readFitsBase(){
}

readFitsBase::~readFitsBase(){
}

void readFitsBase::inputFits(char* input){

  if ( fits.read_stream( input ) < 0 ) {
    std::cout << "[ERROR] cannot open: " << input << std::endl;
    exit(1);
  }
  
  sio.eprintf("[INFO] Specify index.\n");
  for (long i=0 ; i < fits.length() ; i++ ) {
    if ( fits.hdutype(i) == FITS::BINARY_TABLE_HDU ||
	 fits.hdutype(i) == FITS::ASCII_TABLE_HDU ) {
      sio.eprintf("Index:%ld  Name:%s\n", i, fits.hduname(i));
    }
  }
}

long readFitsBase::getRowNum(){
  long rownum = fits.table( "FILTER" ).row_length();
  return rownum;
}
