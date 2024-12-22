#ifndef TIME_H
#define TIME_H

#include <string>

class Time {
  private:
    int hours_;
    int minutes_;
    void StringToTime(std::string time);
  public:
    class Invalid{};
    Time(std::string time);
    Time();
    void addTime(Time time);
    void setTime(std::string time);
    int getHours();
    int getMinutes();
    std::string to_string();
};

#endif // !#ifndef TIME_H TIME_H


