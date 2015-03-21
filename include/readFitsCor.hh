#include <sli/stdstreamio.h>
#include <sli/tstring.h>
#include <sli/fitscc.h>

#include "readFitsBase.hh"

using namespace sli;


class readFitsCor : public virtual readFitsBase{

public:
  virtual double getCor(int id);
};
