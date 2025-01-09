#ifndef MANUAL_H
#define MANUAL_H

#include "Device.h"

class Manual : public Device{
  private:
    //nothing

  public:

    Manual(const std::string& id, const std::string& name, double power, Time start, Time finish);
    
};

#endif // MANUAL_H
