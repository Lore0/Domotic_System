#include "../include/PrefixCycle.h"
#include <iostream>

PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Time start, Time cycleDuration)
    :   Device(id, name, power, start, start.addTime(cycleDuration)),
        cycleDuration_(cycleDuration)

{
    //left intentionally blank
}

void PrefixCycle::setStartFinish(Time start){
    start_ = start;
    finish_ = start.addTime(getDuration());
    std::cout<<finish_.to_string()<<std::endl;
}

Time PrefixCycle::getDuration(){
    return cycleDuration_;
}




