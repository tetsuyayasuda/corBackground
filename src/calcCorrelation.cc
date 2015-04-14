#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "calcCorrelation.hh"

calcCorrelation::calcCorrelation(){
}

calcCorrelation::~calcCorrelation(){
}

void calcCorrelation::init(){

  // Initial Parameters
  mkfNum = 0;
  srcMkfId = -1;
  bg1MkfId = -1;
  bg2MkfId = -1;
  timeFlag = 0;
  crossOffset = 25000;
  searchOffset = 1000;
  searchResion = 200;

}

double calcCorrelation::inputFloat(){
}

vector calcCorrelation::inputListASCII(){
}

