#ifndef MANUAL_H
#define MANUAL_H

#include "Device.h"

class Manual : public Device{
  private:
    Time start_;
    Time finish_;
  public:
    Manual(const std::string& id, const std::string& name, double power, Time start, Time finish);
    Time getStart();
    Time getFinish();
    void setStart(Time start);
    void setFinish(Time finish);
};

#endif
