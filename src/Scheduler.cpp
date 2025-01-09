#include "../include/Scheduler.h"
#include "../include/PrefixCycle.h"
#include <algorithm>
#include <iostream>
using namespace std;

// cout in setdevice time

Scheduler::Scheduler(double maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("00:00")
{
    // nothing
}

string Scheduler::setOn(Device *device) // se è già in waiting bisogna cercarlo e toglierlo
{
    string log = "";
    int targetIndex = searchDevice(waiting_, device);
    int i = 0;
    if (inUsePower + device->getPowerConsumption() + MAX_POWER >= 0)
    {
        while (i < running_.size() && running_[i]->getFinish() < device->getFinish())
            i++;
        device->turnOn();
        //device->setStart(currTime);
        Time temp = currTime;
        // if (PrefixCycle* p =dynamic_cast<PrefixCycle*>(device))
        //     device->setFinish(temp.addTime(device->getFinish()));
        inUsePower += device->getPowerConsumption();
        running_.insert(running_.begin() + i, device);
        log = "\n" + getStringCurrTime() + " Il dispositivo " + device->getName() + " si è acceso"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è acceso
    }
    else
    {
        cout << "no ghe xe corente" << endl;
    } // cosa fare in caso non c'è abbastanza potenza
    if (targetIndex != -1)
    {
        waiting_.erase(waiting_.begin() + targetIndex);
    }
    return log;
}

string Scheduler::setOff(Device *device)
{
    int targetIndex = searchDevice(running_, device);
    running_.erase(running_.begin() + targetIndex);
    device->turnOff();
    device->setHasTimer(false);
    device->setFinish(currTime);
    inUsePower -= device->getPowerConsumption();
    string log = "\n" + getStringCurrTime() + " Il dispositivo " + device->getName() + " si è spento"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è spendo
    Device temp{device->getId(), device->getName(), device->getPowerConsumption(), device->getStart(), device->getFinish()};
    finished_.push_back(temp); // da fare con pane
    return log;
}

string Scheduler::setDeviceTimer(Device *device)
{
    int i = 0;
    device->setHasTimer(true);
    while (i < waiting_.size() && waiting_[i]->getFinish() < device->getFinish())
        i++;
    waiting_.insert(waiting_.begin() + i, device);
    return getStringCurrTime() + " Impostato un timer per il dispositivo " + device->getName() + " dalle " + device->getStart().to_string() + " alle " + device->getFinish().to_string(); //[13:00] Impostatountimerper ildispositivo‘${DEVICENAME}’dalleXX:YYalleZZ:WW
}

string Scheduler::removeTimerFromWaiting(Device *device)
{ // toglie il timer da waiting
    int targetIndex = searchDevice(waiting_, device);
    device->setHasTimer(false);
    if (targetIndex == -1)
        return getStringCurrTime() + " Errore: dispositivo non in attesa";
    waiting_.erase(waiting_.begin() + targetIndex);
    return getStringCurrTime() + " Rimosso il timer dal dispositivo " + device->getName(); // [13:00]Rimossoil timerdaldispositivo‘${DEVICENAME}’
}

string Scheduler::removeTimerFromRunning(Device *device) // toglie il timer da running-- > va all'infinito
{
    int targetIndex = searchDevice(running_, device); // cerco il dispositivo in running
    if (targetIndex == -1)                            // se non c'è do errore
        return getStringCurrTime() + " Errore: dispositivo non in funzione";
    device->setFinish(Time("24:00"));
    device->setHasTimer(false);
    running_.erase(running_.begin() + targetIndex);
    running_.push_back(device);
    // non so perchè avevo fatto la roba sotto
    // int i = running_.size()-1;    //setto i all'ultimo elemento
    //  while (i >= 0 && running_.at(i)->getFinish() > Time("24:00"))   //finchè i non punta al primo elemento e
    //      i--;
    //  Device* temp = running_[targetIndex];
    //  running_[targetIndex] = running_[i];
    //  running_[i] = temp;
    return getStringCurrTime() + " Rimosso il timer dal dispositivo " + device->getName(); // [13:00]Rimossoil timerdaldispositivo‘${DEVICENAME}’
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
    // cout << waiting_.at(0).getName() << endl;
    while (!(currTime > time)) //
    {
        if ((running_.empty() && waiting_.empty())) // controllo di fine (entrambe liste vuote)
        {
            currTime = time;
            break;
        }
        else if (running_.empty() || waiting_.empty()) // se una delle due è vuote
        {
            if (running_.empty()) // se running è vuota
            {
                if (waiting_.at(0)->getStart() > time) // se primo waiting è già pèiù grande
                {
                    currTime = time;
                    break;
                }
                else // waiting no vuota e un dispositivo si deve avviare
                {
                    currTime = waiting_.at(0)->getStart();
                    log += setOn(waiting_.at(0));
                }
            }
            else // waiting vuota
            {
                if (running_.at(0)->getFinish() > time) // se primo running è già pèiù grande
                {
                    currTime = time;
                    break;
                }
                else // running no vuota e un dispositivo si deve avviare
                {
                    currTime = running_.at(0)->getFinish();
                    log += setOff(running_.at(0));
                }
            }
        }
        else // entrambe running waiting NO vuote
        {
            if (waiting_.at(0)->getStart() < running_.at(0)->getFinish()) // waiting<running
            {
                currTime = waiting_.at(0)->getStart();
                log += setOn(waiting_.at(0));
            }
            else // waiting>=running
            {
                currTime = running_.at(0)->getFinish();
                log += setOff(running_.at(0));
            }
        }
    }
    return log + "\n" + getStringCurrTime() + "L'orario attuale è " + getCurrTime().to_string();
}

string Scheduler::resetTime()
{
    currTime.setTime("00:00");
    for (Device *d : running_)
    {
        if (d->hasTimer())
            setDeviceTimer(d);
        else
        {
            d->setHasTimer(false);
            d->turnOff();
        }
    }
    running_.clear();
    finished_.clear();
    return getStringCurrTime() + " L'orario attuale è 00:00"; //[00:00]L’orarioattualeè00:00
}

string Scheduler::resetTimers() // da rivedere
{
    string log = "";
    for (auto &device : waiting_)
    {
        log += removeTimerFromWaiting(device);
    }
    for (auto &device : running_)
    {
        log += removeTimerFromRunning(device);
    }
    return log;
}

string Scheduler::showAll()
{

    return "";
}

double Scheduler::showDevice(Device *device)
{
    double tothour = 0;
    int targetIndex = searchDevice(running_, device);
    if (targetIndex != -1)
    {
        tothour += currTime.getDistance(running_.at(targetIndex)->getStart());
    }
    int i = 0;
    while (i < finished_.size())
    {
        if (device->getId() == finished_.at(i).getId())
            tothour += finished_.at(i).getFinish().getDistance(finished_.at(i).getStart());
        i++;
    }
    return tothour * device->getPowerConsumption();
}

int searchDevice(vector<Device *> list, Device *target)
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
