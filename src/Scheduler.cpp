#include "../include/Scheduler.h"
#include <algorithm>
using namespace std;

Scheduler::Scheduler(int maxPower) 
    : MAX_POWER(maxPower), inUsePower(0) {
    //nothing
}

void Scheduler::setOn(Device &device)   //controllo se c'è già su waiting?
{
    if (running_.empty())
        running_.push_back(device);
    else
    {
        int i=0;
        while (i < running_.size() && running_[i].getFinish() < device.getFinish())
            i++; 
        if(inUsePower+device.getPowerConsumption()<=MAX_POWER){ 
            running_.insert(running_.begin() + i, device);
            inUsePower+=device.getPowerConsumption();
        }  //fare log
           //cosa fare in caso non c'è abbastanza potenza
    }
}

void Scheduler::setOff(Device &device) {
    int targetIndex=searchDevice(running_,device);
    running_.erase(running_.begin()+targetIndex);
    Device temp{device.getId(),device.getName(),device.getPowerConsumption(),device.getStart(),device.getFinish()}; //da cambiare con costruttore di manual
}
void Scheduler::setDeviceTimer(Device &device_) {}
void Scheduler::removeFromWaiting(Device &device_) {}
// show
// showdevice;
void Scheduler::setTime(Time &time_) {}
void Scheduler::resetTime() {}
void Scheduler::resetTimers() {}

int searchDevice(std::vector<Device>& list, Device& target) {    //serve == su Device, ritorna index
    
}