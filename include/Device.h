#ifndef DEVICE_H
#define DEVICE_H

class Device
{
    protected:
        std::string name_;
        std::string id_;
        double power_;
        bool status_; //on = true , off = false
        Time start_;
        Time finish_;
        //costruttore
        Device(const std::string& name, const std::string& id, double power, Time start, Time finish);
    public:
        
        //get
        bool getStatus() const;

        const std::string& getName() const;

        const std::string& getId() const;

        double getPowerConsumption() const;

        Time getStart() const;

        Time getFinish() const;

        //accendo il device
        virtual void turnOn();

        //spengo il device
        virtual void turnOff();

        Device(const Device&) = delete;

        Device& operator=(const Device&) = delete
}

#endif