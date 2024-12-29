#ifndef PREFIXCYCLE_H
#define PREFIXCYCLE_H

#include "Device.h"

class PrefixCycle : public Device{
  private:
    Time cycleDuration_;
  public:
    PrefixCycle(const std::string& id, const std::string& name, double power, Time start, Time cycleDuration);
    void setStartFinish(Time start);
    Time getDuration();
};

#endif
