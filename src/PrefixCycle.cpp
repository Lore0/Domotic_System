#include "../include/PrefixCycle.h"

PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Time cycleDuration)
    :   Device(id, name, power),
        cycleDuration_(cycleDuration)
{
    //left intentionally blank
}

Time PrefixCycle::getDuration(){
    return cycleDuration_;
}




