#ifndef READFITS_H
#define READFITS_H

//*****************************************
//                readFits
// 
//      2012-11-02   T.Yasuda    ver0.0
//
//-----------------------------------------
//              readFitsBase
//               |        |
//       readFitsTime   readFitsCor
//                |      |
//                readFits
//


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsTime.hh"
#include "readFitsCor.hh"

using namespace sli;


class readFits : public virtual readFitsTime, 
		 public virtual readFitsCor{

public:
  readFits();
  ~readFits();

protected:

};

#endif
