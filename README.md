# corBackground	


## Overview
corBackground is a C++ code to search the time region of similar cutoff rigidity (COR) level with an arbitrary time.
It calculate cross correlation among time variations of COR of three days.


## Requirement
- C++ compiler
- Cmake
- SLI Libraries/SFITSIO [http://www.ir.isas.jaxa.jp/~cyamauch/sli/index.html]


## Test Environment
- Mac OS X  10.9.5
- SLLIB     1.4.2
- SFITSIO   1.4.2


## Usage
### Compile
$ ls  
 source/  
$ mkdir build run  
$ cd build  
$ cmake ../source  
$ make  
$ cd ../run  
$ ln -s ../build/corBackground


### Preparation
You should prepare Suzaku mkf files (FITS), those list file (ASCII), and source time as Suzaku time (aste time).

    1. Suzaku mkf files (FITS)  
    Download mkf files (ae*.mkf.gz) from Suzaku public data archive
    [http://www.darts.isas.jaxa.jp/astro/suzaku/public_seq.html].

    2. mkf list file (ASCII: mkfList.txt)  
    $ find /path/to/mkfs/ae*mkf.gz > mkfList.txt

    3. source time (observed date)
    You can know aste time as the source time using the useful tool, xtime 
    [https://heasarc.gsfc.nasa.gov/cgi-bin/Tools/xTime/xTime.pl].


### Run
    (Usage)
    ./corBackground [asteTime] [mkfList] [option:day]
    
    (Example)
    $ ls
    corBackground   mkfList.txt
    $ ./corBackground 285835516.9 mkfList.txt 51
    

### Outputs
Please check outputs, whether calculation was appropriate or not.
- crossCurveFirst.qdp:  corrected COR curves.
- crossCurveSecond.qdp: corrected COR curves.
- results.txt


## Licence
You can use this software under the MIT license.


## Author
- Tetsuya Yasuda ( yasuda AT heal.phy.saitama-u.ac.jp )
- Saitama University 

