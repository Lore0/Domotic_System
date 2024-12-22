#define TIME_H
#ifndef TIME_H

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

#endif // !#ifndef TIME_H TIME_H


