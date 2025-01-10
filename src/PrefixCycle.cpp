#include "../include/PrefixCycle.h"

PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Clock start, Clock cycleDuration)
    :   Device(id, name, power, start, start),
        cycleDuration_(cycleDuration)

{
  // left intentionally blank
}

void PrefixCycle::setStart(Clock start){
    start_ = start;
    finish_ = start.addTime(getDuration());
}

Clock PrefixCycle::getDuration() const{
    return cycleDuration_;
}




