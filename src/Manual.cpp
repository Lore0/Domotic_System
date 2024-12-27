#include "../include/Manual.h"

Manual::Manual(const std::string& id, const std::string& name, double power, Time start, Time finish)
    :   Device(id, name, power), 
        start_(start), 
        finish_(finish)
{
    //left intentionally blank
}

Time Manual::getStart(){
    return start_;
}

Time Manual::getFinish(){
    return finish_;
}

void Manual::setStart(Time start){
    start_ = start;
}

void Manual::setFinish(Time finish){
    finish_ = finish;
}