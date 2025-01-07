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

string Scheduler::setOn(Device *device) // se è già in waiting bisogna cercarlo e toglierlo
{
    string log = "";
    int targetIndex = searchDevice(waiting_, device);
    int i = 0;
    if (inUsePower + device->getPowerConsumption() <= MAX_POWER)
    {
        while (i < running_.size() && running_[i]->getFinish() < device->getFinish())
            i++;
        device->turnOn();
        device->setStart(currTime);
        Time temp = currTime;
        if (targetIndex == -1)
            device->setFinish(temp.addTime(device->getFinish()));
        inUsePower += device->getPowerConsumption();
        running_.insert(running_.begin() + i, device);
        log = "\n" + getStringCurrTime() + " Il dispositivo " + device->getName() + " si è acceso"; //[13:00] Il dispositivo ‘${DEVICENAME}’ si è acceso
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
    while (i < waiting_.size() && waiting_[i]->getFinish() < device->getFinish())
        i++;
    waiting_.insert(waiting_.begin() + i, device);
    return getStringCurrTime() + " Impostato un timer per il dispositivo " + device->getName() + " dalle " + device->getStart().to_string() + " alle " + device->getFinish().to_string(); //[13:00] Impostatountimerper ildispositivo‘${DEVICENAME}’dalleXX:YYalleZZ:WW
}

string Scheduler::removeTimerFromWaiting(Device *device)
{ // toglie il timer da waiting
    int targetIndex = searchDevice(waiting_, device);
    if (targetIndex == -1)
        return getStringCurrTime() + " Errore: dispositivo non in attesa";
    waiting_.erase(waiting_.begin() + targetIndex);
    return getStringCurrTime() + " Rimosso il timer dal dispositivo " + device->getName(); // [13:00]Rimossoil timerdaldispositivo‘${DEVICENAME}’
}

string Scheduler::removeTimerFromRunning(Device *device)    //toglie il timer da running-- > va all'infinito 
{
    int targetIndex = searchDevice(running_, device);   //cerco il dispositivo in running    
    if (targetIndex == -1)  //se non c'è do errore
        return getStringCurrTime() + " Errore: dispositivo non in funzione";
    device->setFinish(Time("25:00"));
    running_.erase(running_.begin()+targetIndex);
    running_.push_back(device);
    //non so perchè avevo fatto la roba sotto
    //int i = running_.size()-1;    //setto i all'ultimo elemento
    // while (i >= 0 && running_.at(i)->getFinish() > Time("24:00"))   //finchè i non punta al primo elemento e 
    //     i--;
    // Device* temp = running_[targetIndex];
    // running_[targetIndex] = running_[i];
    // running_[i] = temp;
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
    while (!(currTime > time))
    {
        cout << "a" << endl;
        if ((running_.empty() && waiting_.empty()) /*(running_.at(0).getFinish() > time || waiting_.at(0).getStart() > time)*/) // controllo di fine (entrambe liste vuote)
        {
            cout << "stopvuote" << endl;
            currTime = time;
            break;
        }
        if (running_.empty() || waiting_.empty())
        {
            cout << "ba" << endl;
            if (running_.empty())
            {
                if (waiting_.at(0)->getStart() > time)
                {
                    cout << "stopwaitalta" << endl;
                    currTime = time;
                    break;
                }
                cout << "aaa" << endl;
                if (!waiting_.empty() && waiting_.at(0)->getStart() < time)
                {
                    cout << "bb" << endl;
                    currTime = waiting_.at(0)->getStart();
                    // cout<<waiting_.at(0)->getName()+""+waiting_.at(0)->getFinish().to_string()<<endl;
                    log += setOn(waiting_.at(0));
                    // waiting_.erase(waiting_.begin()); ->se non lo faccio su seton
                }
            }
            else if (waiting_.empty())
            {
                if (running_.at(0)->getFinish() > time)
                {
                    cout << "stoprunalta" << endl;
                    currTime = time;
                    break;
                }
                cout << "aab" << endl;
                if (!running_.empty() && running_.at(0)->getFinish() < time)
                {
                    currTime = running_.at(0)->getFinish();
                    cout << "bc" << endl;
                    log += setOff(running_.at(0));
                }
            }
        }
        else
        {
            cout << "cc" << endl;
            cout << waiting_.at(0)->getName() << endl;
            if (waiting_.at(0)->getStart() < running_.at(0)->getFinish())
            {
                cout << "cd" << endl;
                currTime = waiting_.at(0)->getStart();
                log += setOn(waiting_.at(0));
            }
            else
            {
                cout << "ce" << endl;
                currTime = running_.at(0)->getFinish();
                log += setOff(running_.at(0));
            }
        }
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
    // currTime.setTime("0:0");
    // for (Device &d : running_)
    // {
    //     setDeviceTimer(d);
    // }
    // for (Device &d : waiting_)
    // {
    //     setDeviceTimer(d);
    // } // controllare se il dispositivo è stato acceso più di una volta
    // running_.clear();
    // finished_.clear();
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

string Scheduler::showAll()
{
    return "";
}

string Scheduler::showDevice(Device *device)
{
    return "";
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
