#ifndef PREFIXCYCLE_H
#define PREFIXCYCLE_H

#include "Device.h"

class PrefixCycle : public Device{
  private:
  
    Clock cycleDuration_;

  public:

    PrefixCycle(const std::string& id, const std::string& name, double power, Clock start, Clock cycleDuration);

    void setStart(Clock start);

    Clock getDuration() const;

    //void setStart(Time start) = delete;
    //void setFinish(Time finish) = delete
};

#endif // PREFIXCYCLE_H
