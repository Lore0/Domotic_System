#include "../include/Time.h"
#include <string>
#include <sstream>

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

void Time::StringToTime(const string& time){
  char div;
  int hours, minutes;
  istringstream stream(time); //stream di time

  if (!(stream >> hours >> div >> minutes) || div != ':' || hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
      throw Invalid();
  }

  hours_ = hours;
  minutes_ = minutes;
}

Time Time::addTime(const Time& time){
  if(time.getHours()==-1) return *this;
  minutes_ += time.minutes_;
  hours_ += time.hours_ + (minutes_/60);
  minutes_ %= 60;
  hours_ %= 24;
  return *this;
}

int Time::getHours() const{
  return hours_;
}

int Time::getMinutes() const{
  return minutes_;
}

void Time::setTime(const string& time){
  StringToTime(time);
}

string Time::to_string() const{ //TODO
  string hours, minutes, clock = "";

  if(hours_ < 10){
    hours = "0";
  }
  hours = hours + std::to_string(hours_);

  if(minutes_ < 10){
    minutes = "0";
  }
  minutes = minutes + std::to_string(minutes_);

  clock = hours+":"+minutes;
  return clock;
}

Time& Time::operator=(const Time &time){
  hours_=time.hours_;
  minutes_=time.minutes_;
  return *this;
}


bool operator>(const Time& t1, const Time& t2){
  if(t1.getHours() == t2.getHours()){
    if(t1.getMinutes() > t2.getMinutes()){
      return true;
    }else{
      return false;
    }
  }
  if(t1.getHours() > t2.getHours()){
    return true;
  }else{
    return false;
  }
}

bool operator<(const Time& t1, const Time& t2){
  if(t1.getHours() == t2.getHours()){
    if(t1.getMinutes() < t2.getMinutes()){
      return true;
    }else{
      return false;
    }
  }
  if(t1.getHours() < t2.getHours()){
    return true;
  }else{
    return false;
  }
}

bool operator==(const Time& t1, const Time& t2){
  if(t1.getHours() == t2.getHours() && t1.getMinutes() == t2.getMinutes()){
    return true;
  }else{
    return false;
  }
}