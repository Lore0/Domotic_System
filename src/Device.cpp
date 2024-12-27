#include "../include/Device.h"
#include <iostream>

using namespace std;

Device::Device(const string& id, const string& name, double power)
    :   id_(id),
        name_(name),
        power_(power),
        status_(false)
{
    //left intentionally blank
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
