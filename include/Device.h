#define DEVICE_H
#ifndef DEVICE_H

class Device
{
    protected:
        std::string name_;
        std::string id_;
        double power_;
        bool status_; //on = true , off = false
        
    
    public:
        //costruttore
        Device(const std::string& name, const std::string& id, double power);
        
        //get
        bool getStatus() const;

        const std::string& getName() const;

        const std::string& getId() const;

        double getPowerConsumption() const;

        //accendo il device
        virtual void turnOn();

        //spengo il device
        virtual void turnOff();
}

#endif