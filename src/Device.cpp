#include "../include/Device.h"
#include <iostream>

using namespace std;

Device::Device(const string& id, const string& name, double power, Time start, Time finish)
    :   id_(id),
        name_(name),
        power_(power),
        start_(start),
        finish_(finish),
        status_(false)
{
    //left intetionally blank
}

const string& Device::getId() const{
    return id_;
}


const string& Device::getName() const{
    return name_;
}

double Device::getPowerConsumption() const{
    return power_;
}

Time Device::getStart() const{
    return start_;
}

Time Device::getFinish() const{
    return finish_;
}

bool Device::getStatus() const{
    return status_;
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
