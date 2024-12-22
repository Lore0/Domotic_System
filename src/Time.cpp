#include "../include/Time.h"
#include <string>
#include <iostream>
#include <sstream>
/**
class Time {
  public:
    Time(std::string time);
    Time();
    void addTime(Time time);
    void setTime(std::string time);
    int getHours();
    int getMinutes();
    std::string toString();
    
  private:
    int hours;
    int minutes;
    void StringToTime(std::string time);
}
*/

using namespace std;

Time::Time(std::string time){
  StringToTime(time);
}

Time::Time()
  : hours_(0), 
    minutes_(0)
{
    //left intentionally blank
}

void Time::StringToTime(string time){
  char div;
  int hours, minutes;
  istringstream stream(time); //stream di time

  if (!(stream >> hours >> div >> minutes) || div != ':' || hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
      throw Invalid();
  }

  hours_ = hours;
  minutes_ = minutes;
}

void Time::addTime(Time time){
  minutes_ += time.minutes_;
  hours_ += time.hours_ + (minutes_/60);
  minutes_ %= 60;
  hours_ %= 24;
}

int Time::getHours(){
  return hours_;
}

int Time::getMinutes(){
  return minutes_;
}

void Time::setTime(string time){
  StringToTime(time);
}

string Time::to_string(){
  return;
}
