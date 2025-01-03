#include "../include/Scheduler.h"
#include <algorithm>
using namespace std;

Scheduler::Scheduler(int maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("0:0")
{
    // nothing
}

string Scheduler::setOn(Device &device) // controllo se c'è già su waiting?
{
    string log=getCurrTime(currTime);
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
    return log;
}

string Scheduler::setOff(Device &device)
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

string Scheduler::setDeviceTimer(Device &device)
{
    int i;
    while (i < waiting_.size() && waiting_[i].getFinish() < device.getFinish())
        i++;
    waiting_.insert(waiting_.begin() + i, device);
}

string Scheduler::removeTimerFromWaiting(Device &device)
{ // toglie il timer da waiting
    int targetIndex = search(waiting_, device);
    if (targetIndex == -1)
        return;
    waiting_.erase(waiting_.begin() + targetIndex);
}

string Scheduler::removeTimerFromRunning(Device &device)
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
string Scheduler::setTime(Time &time)
{
    if (currTime > time)
        return; // fare qualcosa se l'ora non è valida
    // controllare se lista vuota
    // controllaere qual'è minore
    // se sono uguali tolgo prima da running
    // fino a che non sono entrambi < time
    while ((!running_.empty() || !waiting_.empty()) && (!(running_.at(0).getFinish() > time) || !(waiting_.at(0).getStart() > time)))
    {
        if (!running_.empty())
        {
            while (!(running_.at(0).getFinish() > time) && (waiting_.empty() || !(running_.at(0).getFinish() > waiting_.at(0).getStart())))
            {
                currTime = running_.at(0).getFinish();
                setOn(running_.at(0));
            }
        }
        if (!waiting_.empty())
        {
            while (!(waiting_.at(0).getStart() > time) && (running_.empty() || !(waiting_.at(0).getStart() > running_.at(0).getFinish())))
            {
                currTime = waiting_.at(0).getStart();
                setOff(waiting_.at(0));
            }
        }
    }
}

string Scheduler::resetTime()
{
    currTime.setTime("0:0");
    for_each(running_.begin(), running_.end(), setDeviceTimer);
    for_each(finished_.begin(), finished_.end(), setDeviceTimer); // controllare se il dispositivo è stato acceso più di una volta
    running_.clear();
    finished_.clear();
}

string Scheduler::resetTimers()
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

string getCurrTime(Time currTime){
    return "["+currTime.to_string()+"] L'orario attuale è "+currTime.to_string(); //[13:00]L’orarioattualeè13:00
}
