#include "../include/Manual.h"

Manual::Manual(const std::string& id, const std::string& name, double power, Time start, Time finish)
    :   Device(id, name, power, start, finish)
{
    //left intentionally blank
}

void Manual::setStart(Time start){
    start_ = start;
}

void Manual::setFinish(Time finish){
    finish_ = finish;
}