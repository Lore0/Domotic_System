#ifndef TIME_H
#define TIME_H

#include <string>

class Time {
  private:
    int hours_;

    int minutes_;

    void StringToTime(const std::string& time);

  public:
    class Invalid{};

    explicit Time(const std::string time);

    Time();

    Time addTime(const Time& time);

    void setTime(const std::string& time);

    int getHours() const;

    int getMinutes() const;

    std::string to_string() const;

    Time& operator=(const Time &time);
};

bool operator>(const Time& t1, const Time& t2);
bool operator<(const Time& t1, const Time& t2);
bool operator==(const Time& t1, const Time& t2);

#endif // TIME_H
