#include "../include/Clock.h"

using namespace std;

Clock::Clock(std::string time) { StringToTime(time); }

Clock::Clock() : hours_(0), minutes_(0) {
  // left intentionally blank
}

void Clock::StringToTime(const string& time) {
  char div;
  int hours, minutes;
  istringstream stream(time);  // stream di time

  if (!(stream >> hours >> div >> minutes) || div != ':' || hours < 0 ||hours > 24 || minutes < 0 || minutes >= 60) {
    throw InvalidTimeFormat("ERRORE: l'orario deve essere esplicitato come hh:mm.");
  }

  hours_ = hours;
  minutes_ = minutes;
}

Clock Clock::addTime(const Clock& time) {
  if (time.getHours() == -1) return *this;
  minutes_ += time.minutes_;
  hours_ += time.hours_ + (minutes_ / 60);
  minutes_ %= 60;
  hours_ %= 24;
  return *this;
}

int Clock::getHours() const { return hours_; }

int Clock::getMinutes() const { return minutes_; }

void Clock::setTime(const string& time) { StringToTime(time); }

string Clock::to_string() const {  // TODO
  string hours, minutes, clock = "";

  if (hours_ < 10) {
    hours = "0";
  }
  hours = hours + std::to_string(hours_);

  if (minutes_ < 10) {
    minutes = "0";
  }
  minutes = minutes + std::to_string(minutes_);

  clock = hours + ":" + minutes;
  return clock;
}

double Clock::getDistance(const Clock& time) const {
  if (*this < time) {
    throw invalid_argument(
        "ERRORE: il parametro passato deve essere maggiore.");
  }
  double thisMinutes = hours_ * 60 + minutes_;
  double timeMinutes = time.getHours() * 60 + time.getMinutes();

  double difference = thisMinutes - timeMinutes;

  return difference / 60;
}

Clock& Clock::operator=(const Clock& time) {
  hours_ = time.hours_;
  minutes_ = time.minutes_;
  return *this;
}

bool operator>(const Clock& t1, const Clock& t2) {
  if (t1.getHours() == t2.getHours()) {
    if (t1.getMinutes() > t2.getMinutes()) {
      return true;
    } else {
      return false;
    }
  }
  if (t1.getHours() > t2.getHours()) {
    return true;
  } else {
    return false;
  }
}

bool operator<(const Clock& t1, const Clock& t2) {
  if (t1.getHours() == t2.getHours()) {
    if (t1.getMinutes() < t2.getMinutes()) {
      return true;
    } else {
      return false;
    }
  }
  if (t1.getHours() < t2.getHours()) {
    return true;
  } else {
    return false;
  }
}

bool operator==(const Clock& t1, const Clock& t2) {
  if (t1.getHours() == t2.getHours() && t1.getMinutes() == t2.getMinutes()) {
    return true;
  } else {
    return false;
  }
}
