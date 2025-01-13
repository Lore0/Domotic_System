//COMPAGNO FEDERICO

#include "../include/Clock.h"

using namespace std;

/**
 * @brief Constructor that initializes the clock with a string time.
 * @param time A string representing the time in hh:mm format.
 */
Clock::Clock(std::string time) { 
  StringToTime(time); 
}

/**
 * @brief Default constructor that initializes the clock to 00:00.
 */
Clock::Clock() : hours_(0), minutes_(0) {
  // Left intentionally blank
}

/**
 * @brief Converts a string representation of time to hours and minutes.
 * @param time A string representing the time in hh:mm format.
 * @throws InvalidTimeFormat if the time format is incorrect.
 */
void Clock::StringToTime(const string& time) {
  char div;
  int hours, minutes;
  istringstream stream(time); 
  if (!(stream >> hours >> div >> minutes) || div != ':' || hours < 0 || hours > 24 || minutes < 0 || minutes >= 60 || time.length() != 5) {
    throw InvalidTimeFormat("ERRORE: l'orario deve essere esplicitato come hh:mm.");
  }
  hours_ = hours;
  minutes_ = minutes;
}

/**
 * @brief Adds another Clock object's time to the current object.
 * @param time A Clock object representing the time to be added.
 * @return The updated Clock object with the added time.
 */
Clock Clock::addTime(const Clock& time) {
  if (time.getHours() == -1) return *this;
  minutes_ += time.minutes_;
  hours_ += time.hours_ + (minutes_ / 60);
  minutes_ %= 60;
  hours_ %= 24;
  return *this;
}

/**
 * @brief Returns the hours of the clock.
 * @return The hours of the clock.
 */
int Clock::getHours() const { 
  return hours_; 
}

/**
 * @brief Returns the minutes of the clock.
 * @return The minutes of the clock.
 */
int Clock::getMinutes() const { 
  return minutes_; 
}

/**
 * @brief Sets the clock time using a string.
 * @param time A string representing the time in hh:mm format.
 */
void Clock::setClock(const string& time) { 
  StringToTime(time);
}

/**
 * @brief Returns a string representation of the time.
 * @return A string representing the time in hh:mm format.
 */
string Clock::to_string() const {
  string hours, minutes, clock = "";
  // If the hours are between [0,9] I add a 0 in front
  if (hours_ < 10) {
    hours = "0";
  }
  hours = hours + std::to_string(hours_);
  // If the minutes are between [0,9] I add a 0 in front
  if (minutes_ < 10) {
    minutes = "0";
  }
  minutes = minutes + std::to_string(minutes_);
  clock = hours + ":" + minutes;
  return clock;
}

/**
 * @brief Calculates the time difference between two Clock objects.
 * @param time A Clock object representing the time to compare.
 * @return The time difference in hours.
 * @throws invalid_argument if the parameter time is greater than the current time.
 */
double Clock::getDistance(const Clock& time) const {
  if (*this < time) {
    throw invalid_argument("ERRORE: il parametro passato deve essere maggiore.");
  }
  double thisMinutes = hours_ * 60 + minutes_;
  double timeMinutes = time.getHours() * 60 + time.getMinutes();
  double difference = thisMinutes - timeMinutes;
  return difference / 60;
}

/**
 * @brief Assignment operator for the Clock class.
 * @param time A Clock object representing the time to be assigned.
 * @return The updated Clock object.
 */
Clock& Clock::operator=(const Clock& time) {
  hours_ = time.hours_;
  minutes_ = time.minutes_;
  return *this;
}

/**
 * @brief Compares if one Clock object is greater than another.
 * @param t1 The first Clock object.
 * @param t2 The second Clock object.
 * @return True if t1 is greater than t2, false otherwise.
 */
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

/**
 * @brief Compares if one Clock object is less than another.
 * @param t1 The first Clock object.
 * @param t2 The second Clock object.
 * @return True if t1 is less than t2, false otherwise.
 */
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

/**
 * @brief Compares if one Clock object is equals to another.
 * @param t1 The first Clock object.
 * @param t2 The second Clock object.
 * @return True if t1 is equal to t2, false otherwise.
 */
bool operator==(const Clock& t1, const Clock& t2) {
  if (t1.getHours() == t2.getHours() && t1.getMinutes() == t2.getMinutes()) {
    return true;
  } else {
    return false;
  }
}