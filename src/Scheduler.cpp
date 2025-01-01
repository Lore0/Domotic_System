#include "../include/Scheduler.h"
#include <algorithm>
using namespace std;

Scheduler::Scheduler(int maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("0:0")
{
    // nothing
}

void Scheduler::setOn(Device &device) // controllo se c'è già su waiting?
{
    if (running_.empty())
        running_.push_back(device);
    else
    {
        int i = 0;
        if (inUsePower + device.getPowerConsumption() <= MAX_POWER)
        {
            while (i < running_.size() && running_[i].getFinish() < device.getFinish())
                i++;
            running_.insert(running_.begin() + i, device);
            device.turnOn();
            device.setStart(currTime);
            inUsePower += device.getPowerConsumption();
        } // fare log
          // cosa fare in caso non c'è abbastanza potenza
    }
}

void Scheduler::setOff(Device &device)
{
    int targetIndex = search(running_, device);
    if (device.getStatus())
    {
        // fare qualcosa se il dispositivo non è on
        return;
    }
    running_.erase(running_.begin() + targetIndex);
    device.turnOff();
    device.setFinish(currTime);
    Device temp{device.getId(), device.getName(), device.getPowerConsumption(), device.getStart(), device.getFinish()};
    finished_.push_back(temp);
}

void Scheduler::setDeviceTimer(Device &device)
{
    int i;
    while (i < waiting_.size() && waiting_[i].getFinish() < device.getFinish())
        i++;
    waiting_.insert(waiting_.begin() + i, device);
}

void Scheduler::removeTimerFromWaiting(Device &device)
{ // toglie il timer da waiting
    int targetIndex = search(waiting_, device);
    if (targetIndex == -1)
        return;
    waiting_.erase(waiting_.begin() + targetIndex);
}

void Scheduler::removeTimerFromRunning(Device &device)
{ // toglie il timer da running --> va all'infinito
    int targetIndex = search(running_, device);
    if (targetIndex == -1)
        return;
    int i = running_.size();
    while (i >= 0 && running_.at(i).getFinish() > Time("24:00"))
        i--;
    Device temp = running_[targetIndex];
    running_[targetIndex] = running_[i];
    running_[i] = temp;
}
// show
// showdevice;
void Scheduler::setTime(Time &time)
{
    if(currTime>time) return;   //fare qualcosa se l'ora non è valida
    //controllare se lista vuota
    //controllaere qual'è minore
    //se sono uguali tolgo prima da running
    //fino a che non sono entrambi < time
    while (
        ()
    )
    {
        /* code */
    }
}

void Scheduler::resetTime()
{
    currTime.setTime("0:0");
    for_each(running_.begin(), running_.end(), setDeviceTimer);
    for_each(finished_.begin(), finished_.end(), setDeviceTimer); // controllare se il dispositivo è stato acceso più di una volta
    running_.clear();
    finished_.clear();
}

void Scheduler::resetTimers()
{
    waiting_.clear();
    for_each(running_.begin(), running_.end(), removeTimerFromRunning);
}

int search(vector<Device> &list, Device &target)
{
    // Usa std::find per cercare il dispositivo
    auto iter = find(list.begin(), list.end(), target);

    if (iter != list.end())
    {
        // Calcola e restituisci l'indice
        return distance(list.begin(), iter);
    }
    // Se non trovato, restituisce -1
    return -1;
}
