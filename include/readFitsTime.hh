#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsBase.hh"

using namespace sli;


class readFitsTime : public virtual readFitsBase{
  
public:
  virtual double getTime(int id);
  virtual double getTimeStart();
  virtual double getTimeEnd();
};
