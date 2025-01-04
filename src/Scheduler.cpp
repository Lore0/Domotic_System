#include "../include/Scheduler.h"
#include <algorithm>
#include <iostream>
using namespace std;

// cout in setdevice time

Scheduler::Scheduler(int maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("00:00")
{
    // nothing
}

string Scheduler::setOn(Device &device)
{
    string log = "";
    if (running_.empty())
    {
        running_.push_back(device);
        device.turnOn(); // ste tre istruzioni sono da mettere alla fine
        device.setStart(currTime);
        inUsePower += device.getPowerConsumption();
        log = "\n" + getStringCurrTime() + " Il dispositivo " + device.getName() + " si è acceso"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è acceso
    }
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
            log = "\n" + getStringCurrTime() + " Il dispositivo " + device.getName() + " si è acceso"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è acceso
        }
        // cosa fare in caso non c'è abbastanza potenza
    }
    return log;
}

string Scheduler::setOff(Device &device)
{
    int targetIndex = searchDevice(running_, device);
    running_.erase(running_.begin() + targetIndex);
    device.turnOff();
    device.setFinish(currTime);
    string log = log + "\n" + getStringCurrTime() + " Il dispositivo " + device.getName() + " si è spento"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è spendo
    Device temp{device.getId(), device.getName(), device.getPowerConsumption(), device.getStart(), device.getFinish()};
    finished_.push_back(temp);
    return log;
}

string Scheduler::setDeviceTimer(Device &device)
{
    int i;
    while (i < waiting_.size() && waiting_[i].getFinish() < device.getFinish())
        i++;
    waiting_.insert(waiting_.begin() + i, device);
    return getStringCurrTime() + " Impostato un timer per il dispositivo " + device.getName() + " dalle " + device.getStart().to_string() + " alle " + device.getFinish().to_string(); //[13:00] Impostatountimerper ildispositivo‘${DEVICENAME}’dalleXX:YYalleZZ:WW
}

string Scheduler::removeTimerFromWaiting(Device &device)
{ // toglie il timer da waiting
    int targetIndex = searchDevice(waiting_, device);
    if (targetIndex == -1)
        return getStringCurrTime() + " Errore: dispositivo non in attesa";
    waiting_.erase(waiting_.begin() + targetIndex);
    return getStringCurrTime() + " Rimosso il timer dal dispositivo " + device.getName(); // [13:00]Rimossoil timerdaldispositivo‘${DEVICENAME}’
}

string Scheduler::removeTimerFromRunning(Device &device)
{ // toglie il timer da running --> va all'infinito
    int targetIndex = searchDevice(running_, device);
    if (targetIndex == -1)
        return getStringCurrTime() + " Errore: dispositivo non in funzione";
    int i = running_.size();
    while (i >= 0 && running_.at(i).getFinish() > Time("24:00"))
        i--;
    Device temp = running_[targetIndex];
    running_[targetIndex] = running_[i];
    running_[i] = temp;
    return getStringCurrTime() + " Rimosso il timer dal dispositivo " + device.getName(); // [13:00]Rimossoil timerdaldispositivo‘${DEVICENAME}’
}
// show
// showdevice;
string Scheduler::setTime(Time &time)
{
    if (currTime > time) // ora non valida
        return getStringCurrTime() + " Errore: ora minore a quella attuale";
    // controllare se lista vuota
    // controllaere qual'è minore
    // se sono uguali tolgo prima da running
    // fino a che non sono entrambi < time
    string log = "";
    cout<<waiting_.at(0).getName()<<endl;
    while (!(currTime>time)){
        if((running_.empty() && waiting_.empty())||){}
    }
    /*while ((!running_.empty() || !waiting_.empty()) && (!(running_.at(0).getFinish() > time) || !(waiting_.at(0).getStart() > time)))
    {
        cout << "a" << endl;
        if (!running_.empty())
        {
            while (!(running_.at(0).getFinish() > time) && (waiting_.empty() || !(running_.at(0).getFinish() > waiting_.at(0).getStart())))
            {
                cout << "b" << endl;
                currTime = running_.at(0).getFinish();
                log += setOn(running_.at(0));
            }
        }
        if (!waiting_.empty())
        {
            while (!(waiting_.at(0).getStart() > time) && (running_.empty() || !(waiting_.at(0).getStart() > running_.at(0).getFinish())))
            {
                cout<<"c"<<endl;
                currTime = waiting_.at(0).getStart();
                log += setOff(waiting_.at(0));
            }
        }
    }*/
    return log + "\n" + getStringCurrTime() + "L'orario attuale è " + getCurrTime().to_string();
}

string Scheduler::resetTime()
{
    currTime.setTime("0:0");
    for (Device &d : running_)
    {
        setDeviceTimer(d);
    }
    for (Device &d : waiting_)
    {
        setDeviceTimer(d);
    } // controllare se il dispositivo è stato acceso più di una volta
    running_.clear();
    finished_.clear();
    return getStringCurrTime() + " L'orario attuale è 00:00"; //[00:00]L’orarioattualeè00:00
}

string Scheduler::resetTimers()
{
    string log = "";
    waiting_.clear();
    for (auto &device : running_)
    {
        log += removeTimerFromRunning(device);
    }
    return log;
}

string Scheduler::showAll() {
  return "";
}

string Scheduler::showDevice(Device& device) {
  return "";
}

int searchDevice(vector<Device> &list, Device &target)
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

string Scheduler::getStringCurrTime()
{
    return "[" + currTime.to_string() + "]";
}
Time Scheduler::getCurrTime()
{
    return currTime;
}
