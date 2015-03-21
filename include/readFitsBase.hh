#ifndef READFITSBASE_H
#define READFITSBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

using namespace sli;


class readFitsBase{

public:
  readFitsBase();
  ~readFitsBase();
  void inputFits(char* input);
  long getRowNum();

protected:
  stdstreamio sio;
  fitscc fits;
};

#endif
