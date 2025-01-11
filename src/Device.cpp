#include "../include/Device.h"

using namespace std;

Device::Device(const string& id, const string& name, double power, Clock start, Clock finish)
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

Clock Device::getStart() const{
    return start_;
}

Clock Device::getFinish() const{
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

void Device::setStart(Clock start){
    start_ = start;
}

void Device::setFinish(Clock finish){
    finish_ = finish;
}

void Device::turnOn(){
    if(status_ == true){
        cout<<"Il dispositivo è già ACCESO. \n";
        return;
    }
    status_ = true;
}

void Device::turnOff(){
    if(status_ == false){
        cout<<"Il dispositivo è già SPENTO. \n";
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
