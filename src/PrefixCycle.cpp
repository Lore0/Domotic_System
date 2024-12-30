#include "../include/PrefixCycle.h"

PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Time start, Time cycleDuration)
    :   Device(id, name, power, start, start.addTime(cycleDuration)),
        cycleDuration_(cycleDuration)

{
    //left intentionally blank
}

void PrefixCycle::setStart(Time start){
    start_ = start;
    finish_ = start.addTime(getDuration());
}

Time PrefixCycle::getDuration(){
    return cycleDuration_;
}




