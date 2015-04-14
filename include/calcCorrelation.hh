#ifndef CALCCORRELATION_H
#define CALCCORRELATION_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

class calcCorrelation{

public:
  calcCorrelation();
  ~calcCorrelation();
  void init();

protected:

  // parameters -----------------------------                                   
  double srcAsteTime;
  double bg1AsteTime, bg2AsteTime;
  double bg1AsteStart, bg2AsteStart;
  int mkfNum;
  std::ifstream fSrcAsteTime, fMkfList;
  std::vector<std::string> mkfName;
  std::string name;
  int srcMkfId, bg1MkfId, bg2MkfId;
  int srcRowNum, bg1RowNum, bg2RowNum;
  int timeFlag;
  std::vector<double> srcCor, bg1Cor, bg2Cor;
  int bg1Offset, init_bg1Offset, bg2Offset, init_bg2Offset;
  int init_srcOffset;

  ///// region of cross correlation                                             
  int crossOffset;

  ///// region to seach good time lag                                           
  int searchOffset;
  int searchResion; // +-200
};

#endif
