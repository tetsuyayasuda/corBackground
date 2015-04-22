//**************************************************************************
// The MIT License (MIT)
//
// Copyright (c) 2015 Tetsuya Yasuda
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//**************************************************************************
//
// corBackground.cc 
// 
// T.Yasuda   2012-11-02 ver0.0
// (This code was originated by A.Endo on Perl script.)
//
//**************************************************************************


#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>

#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFits.hh"

using namespace sli;

void inputCheck(int argc){
  // parameter number check
  if( argc != 3 && argc != 4 ){
    std::cout << "ERROR: Invalid Parameter(s)." << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./corBackground [asteTime] [mkfList] [option:day]" << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]){


  /**************/
  /*   Step 1   */
  /**************/
 
  // parameters -----------------------------
  double srcAsteTime;
  double bg1AsteTime, bg2AsteTime;
  double bg1AsteStart, bg2AsteStart;
  int mkfNum = 0;
  //std::ifstream fSrcAsteTime, fMkfList;
  std::vector<std::string> mkfName;
  std::string name;
  int srcMkfId = -1;
  int bg1MkfId = -1;
  int bg2MkfId = -1;
  int srcRowNum, bg1RowNum, bg2RowNum;
  int timeFlag = 0;
  std::vector<double> srcCor, bg1Cor, bg2Cor;
  int bg1Offset, init_bg1Offset;
  int bg2Offset, init_bg2Offset;
  int init_srcOffset;
  double day1 = 86400.0; // a day = 86400 s 
  double days = 51.0;    // 51 days (default)

  ///// region of cross correlation 
  int crossOffset = 25000;

  ///// region to seach good time lag
  int searchOffset = 1000;
  int searchResion = 200; // +-200


  // parameters setting
  inputCheck( argc );
  srcAsteTime = atof( argv[1] );
  std::string fMkfList = (std::string)argv[2];
  if( argc == 4 ){
    days = atof( argv[3] );
  }

  // read mkf list
  std::ifstream filein( fMkfList.c_str() );
  for (std::string line; std::getline(filein, line); ){
    if( fopen( line.c_str(), "r" ) == NULL ){
      std::cout << "No such a file: " << line << ", skipped." << std::endl;
      continue;
    }
    mkfName.push_back( line );
  }
  mkfNum = mkfName.size();

  bg1AsteTime = srcAsteTime - (days * day1); // previous ? days
  bg2AsteTime = srcAsteTime + (days * day1); // post ? days
  

  /*******************/
  /*   Step 2        */
  /* search mkf file */
  /*******************/

  for(int i=0; i<mkfNum; i++){

    // search correspond data -----------------------------
    char* mkfPath = new char[ mkfName[i].length() + 1 ];
    strcpy( mkfPath, mkfName[i].c_str() );

    // initialize -----------------------------------------
    std::cout << "------------  read fits  ------------" << std::endl;
    readFits* fits = new readFits();
    
    // read fits file -------------------------------------
    fits->inputFits( mkfPath );
    
    // get each parameters --------------------------------
    long   rowNum    = fits->getRowNum();
    double timeStart = fits->getTimeStart();
    double timeEnd   = fits->getTimeEnd();
    std::cout << "------------  row number  ------------" << std::endl;
    std::cout << rowNum << std::endl;
    std::cout << "---------  observation time  ----------" << std::endl;
    std::cout << timeStart << " <---> " << timeEnd << std::endl;

    // detection ------------------------------------------
    if( timeStart < srcAsteTime && srcAsteTime < timeEnd ){
      std::cout << "-------------  src mkf file  -------------" << std::endl;
      std::cout << "ID: " << i << std::endl;
      if( srcMkfId == -1 ) srcMkfId = i;
      else{
	std::cout << "[caution] two files are detected: src "  << std::endl; 
	continue;
      }
      timeFlag ++;
      for(int j=1; j<rowNum-1; j++){
	srcCor.push_back( fits->getCor( j ) );
      }
      srcRowNum = rowNum-1;
      init_srcOffset = (int)srcAsteTime - (int)timeStart;
      if( timeFlag == 3 )break;
    }
    
    if( timeStart < bg1AsteTime && bg1AsteTime < timeEnd ){
      std::cout << "------------  pre backgroud  ------------" << std::endl;
      std::cout << "ID: " << i << std::endl;
      if( bg1MkfId == -1 ) bg1MkfId = i;
      else{
	std::cout << "[caution] two files are detected: bg1 "  << std::endl; 
	continue;
      }
      timeFlag ++;
      for(int j=1; j<rowNum-1; j++){
	bg1Cor.push_back( fits->getCor( j ) );
      }
      bg1RowNum = rowNum-1;      
      bg1AsteStart = timeStart;
      init_bg1Offset = (int)bg1AsteTime - (int)timeStart;
      if( timeFlag == 3 )break;
    }
    
    if( timeStart < bg2AsteTime && bg2AsteTime < timeEnd ){
      std::cout << "------------  post backgroud  ------------" << std::endl;
      std::cout << "ID: " << i << std::endl;
      if( bg2MkfId == -1 ) bg2MkfId = i;
      else{
	std::cout << "[caution] two files are detected: bg2 " << std::endl; 
	continue;
      }
      timeFlag ++;
      for(int j=1; j<rowNum-1; j++){
	bg2Cor.push_back( fits->getCor( j ) );
      }
      bg2RowNum = rowNum-1;
      bg2AsteStart = timeStart;
      init_bg2Offset = (int)bg2AsteTime - (int)timeStart;
      if( timeFlag == 3 )break;
    }    
  }
  if( timeFlag != 3 ){
    std::cout << "--------------  ERROR  ---------------" << std::endl;
    std::cout << "  Please prepare pertinent mkf files "  << std::endl;
    std::cout << "  of SRC and/or BGD " << std::endl;
    exit(1);
  }

  
  
  /************************/
  /*   Step 3             */
  /* search good time lag */
  /************************/

  double bg1SumCrossCorre = 1.0e256;
  double bg2SumCrossCorre = 1.0e256;
  double pre_bg1SumCrossCorre = 0.0;
  double pre_bg2SumCrossCorre = 0.0;
  std::ofstream crossCorrelation("crossCorrelation.qdp");
 
  // start cross correlation  ------------------------------------
  int firstLag1, firstLag2;
  std::cout << "init_bg1Offset : " << init_bg1Offset << std::endl;
  std::cout << "init_bg2Offset : " << init_bg2Offset << std::endl;

  /// first search ---------------------
  for(int j=-crossOffset; j<crossOffset; j++){
    pre_bg1SumCrossCorre = 0.0;
    pre_bg2SumCrossCorre = 0.0;
    if(j%500==0) std::cout << "( " << j << " / " << crossOffset << " )" << std::endl;
   
    // back ground 1 ---------------------------------------------
    for(int i=-crossOffset; i<crossOffset; i++){
      pre_bg1SumCrossCorre = pre_bg1SumCrossCorre + fabs( bg1Cor[ init_bg1Offset+j+i ] - srcCor[ init_srcOffset+i ] );
    }
    if( pre_bg1SumCrossCorre < bg1SumCrossCorre ){
      bg1SumCrossCorre = pre_bg1SumCrossCorre;
      firstLag1 = j;
    }


    // back ground 2 ---------------------------------------------
    for(int i=-crossOffset; i<crossOffset; i++){
      pre_bg2SumCrossCorre = pre_bg2SumCrossCorre + fabs( bg2Cor[ init_bg2Offset+j+i ] - srcCor[ init_srcOffset+i ] );
    }

    if( pre_bg2SumCrossCorre < bg2SumCrossCorre ){
      bg2SumCrossCorre = pre_bg2SumCrossCorre;
      firstLag2 = j;
    }
    
    crossCorrelation << j <<" "<< pre_bg1SumCrossCorre <<" "<< pre_bg2SumCrossCorre << std::endl;    
  }
  std::cout << "- first search is done " << std::endl << std::endl;
  crossCorrelation.close();




  /// second search -------------------
  bg1SumCrossCorre = 1.0e256;
  bg2SumCrossCorre = 1.0e256;
  for(int j=firstLag1-searchOffset; j<firstLag1+searchOffset; j++){
    pre_bg1SumCrossCorre = 0.0;
    if( (j-firstLag1)%50==0) std::cout << "( " << j << " / " << j-firstLag1 << " )" << std::endl;
   
    // back ground 1 ---------------------------------------------
    for(int i=-searchResion; i<searchResion; i++){
      pre_bg1SumCrossCorre = pre_bg1SumCrossCorre + fabs( bg1Cor[ init_bg1Offset+j+i ] - srcCor[ init_srcOffset+i ] );
    }
    if( pre_bg1SumCrossCorre < bg1SumCrossCorre ){
      bg1SumCrossCorre = pre_bg1SumCrossCorre;
      bg1Offset = j + init_bg1Offset;
    }
  }

  for(int j=firstLag2-searchOffset; j<firstLag2+searchOffset; j++){
    pre_bg2SumCrossCorre = 0.0;
    if( (j-firstLag2)%50==0) std::cout << "( " << j << " / " << j-firstLag2 << " )" << std::endl;
    // back ground 2 ---------------------------------------------
    for(int i=-searchResion; i<searchResion; i++){
      pre_bg2SumCrossCorre = pre_bg2SumCrossCorre + fabs( bg2Cor[ init_bg2Offset+j+i ] - srcCor[ init_srcOffset+i ] );
    }

    if( pre_bg2SumCrossCorre < bg2SumCrossCorre ){
      bg2SumCrossCorre = pre_bg2SumCrossCorre;
      bg2Offset = j + init_bg2Offset;
    }
    
  }
  std::cout << "- second search is done " << std::endl;


  std::cout << "bg1Offset : " << bg1Offset << std::endl;
  std::cout << "bg1SumCrossCorre : " << bg1SumCrossCorre << std::endl;
  std::cout << "bg2Offset : " << bg2Offset << std::endl;
  std::cout << "bg2SumCrossCorre : " << bg2SumCrossCorre << std::endl;
 
  bg1AsteTime = (double)bg1Offset + (double)bg1AsteStart;
  bg2AsteTime = (double)bg2Offset + (double)bg2AsteStart;



  /**************/
  /*   Step 4   */
  /**************/

  double firstLag1Offset = firstLag1 + init_bg1Offset;
  double firstLag2Offset = firstLag2 + init_bg2Offset;
  std::ofstream crossCurveFirst("crossCurveFirst.qdp");
  for( int i=-crossOffset; i<crossOffset+1; i++ ){
    crossCurveFirst << i <<" "
	       << bg1Cor[ firstLag1Offset+i ] <<" "
	       << srcCor[ init_srcOffset+i ] <<" "
	       << bg2Cor[ firstLag2Offset+i ] <<" "
	       << std::endl;
  }
  crossCurveFirst.close();

  std::ofstream crossCurve("crossCurveSecond.qdp");
  for( int i=-searchOffset; i<searchOffset+1; i++ ){
    crossCurve << i <<" "
	       << bg1Cor[ bg1Offset+i ] <<" "
	       << srcCor[ init_srcOffset+i ] <<" "
	       << bg2Cor[ bg2Offset+i ] <<" "
	       << std::endl;
  }
  crossCurve.close();


  
  double bg1TimeLag = (double)init_bg1Offset - (double)bg1Offset;
  double bg2TimeLag = (double)init_bg2Offset - (double)bg2Offset;

  std::cout << std::endl << std::endl << std::endl;
  std::cout << " ******* Result ******* " << std::endl;
  std::cout << " bgd1 : " << mkfName[ bg1MkfId ] << std::endl;
  std::cout << " bgd2 : " << mkfName[ bg2MkfId ] << std::endl;
  std::cout << " src  : " << mkfName[ srcMkfId ] << std::endl;
  std::cout << " bgd1 [Time lag: first]: " << -firstLag1 << std::endl;
  std::cout << " bgd2 [Time lag: first]: " << -firstLag2 << std::endl;
  std::cout << " bgd1 [Time lag: second]: " << bg1TimeLag << std::endl;
  std::cout << " bgd2 [Time lag: second]: " << bg2TimeLag << std::endl;
  std::cout << " ***** calculated time ****** " << std::endl;
  std::cout << " bgd1 [asteTime]: " << std::setprecision(12) << bg1AsteTime << std::endl;
  std::cout << " bgd2 [asteTime]: " << std::setprecision(12) << bg2AsteTime << std::endl;
  std::cout << " src  [asteTime]: " << std::setprecision(12) << srcAsteTime << std::endl;
  std::cout << " ***** finish ****** " << std::endl;
  
  std::ofstream results("results.txt");
  results  << " ******* Result ******* " << std::endl;
  results  << " bgd1 : " << mkfName[ bg1MkfId ] << std::endl;
  results  << " bgd2 : " << mkfName[ bg2MkfId ] << std::endl;
  results  << " src  : " << mkfName[ srcMkfId ] << std::endl;
  results  << " bgd1 [asteTime]: " << std::setprecision(12) << bg1AsteTime << std::endl;
  results  << " bgd2 [asteTime]: " << std::setprecision(12) << bg2AsteTime << std::endl;
  results  << " src  [asteTime]: " << std::setprecision(12) << srcAsteTime << std::endl;
  results  << " ***** It's done ****** " << std::endl;
  results.close();


  return 0;
}
