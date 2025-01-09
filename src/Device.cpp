#include "../include/Device.h"
#include <iostream>

using namespace std;

Device::Device(const string& id, const string& name, double power, Time start, Time finish)
    :   id_(id),
        name_(name),
        power_(power),
        status_(false),
        hasTimer_(false),
        start_(start),
        finish_(finish)
{
    //left intentionally blank
}

const string& Device::getId() const{
    return id_;
}

const string& Device::getName() const{
    return name_;
}

Time Device::getStart() const{
    return start_;
}

Time Device::getFinish() const{
    return finish_;
}

double Device::getPowerConsumption() const{
    return power_;
}

bool Device::getStatus() const{
    return status_;
}

bool Device::hasTimer() const{
  return hasTimer_;
}

void Device::setHasTimer(bool status) {
  hasTimer_ = status;
}

void Device::setStart(Time start){
    start_ = start;
}

void Device::setFinish(Time finish){
    finish_ = finish;
}

void Device::turnOn(){
    if(status_ == true){
        cout<<"The device is already ON. \n";
        return;
    }
    status_ = true;
}

void Device::turnOff(){
    if(status_ == false){
        cout<<"The device is already OFF. \n";
        return;
    }
    status_ = false;
}

bool operator==(const Device& d1, const Device& d2){
    if(d1.getId().compare(d2.getId()) != 0){
        return false;
    }else{
        return true;
    }
}
