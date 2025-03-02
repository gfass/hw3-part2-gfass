#include "TimeCode.h"
#include <stdexcept>

using namespace std;

// Constructor: Accepts hours, minutes, and seconds and converts them into total seconds.
// This ensures that minutes and seconds do not exceed 59, carrying over excess values properly.
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec) {
    min += sec / 60;  // Convert excess seconds into minutes
    sec = sec % 60;   // Keep only the leftover seconds
    hr += min / 60;   // Convert excess minutes into hours
    min = min % 60;   // Keep only the leftover minutes
    t = ComponentsToSeconds(hr, min, sec); // Store the total time as seconds
}

// Copy Constructor: Creates a new TimeCode object with the same total seconds.
TimeCode::TimeCode(const TimeCode& tc) {
   t = tc.t;
}

// Converts hours, minutes, and seconds into a single total seconds value.
// Ensures minutes and seconds do not exceed 59 before conversion.
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec) {
    if (min >= 60 || sec >= 60) {
        throw invalid_argument("Seconds and minutes have to be LESS than 60!");
    }
    return hr * 3600 + min * 60 + sec;
}

// Breaks up total seconds into hours, minutes, and seconds.
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const {
    hr = t / 3600;
    min = (t % 3600) / 60;
    sec = t % 60;
}

// Updates only the hours component, leaving minutes and seconds unchanged.
void TimeCode::SetHours(unsigned int hours) {
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    t = ComponentsToSeconds(hours, min, sec);
}

// Updates only the minutes component, ensuring it remains valid.
void TimeCode::SetMinutes(unsigned int minutes) {
    if (minutes >= 60) {
        throw invalid_argument("Minutes has to be LESS than 60!");
    }
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    t = ComponentsToSeconds(hr, minutes, sec);
}

// Updates only the seconds component, ensuring it remains valid.
void TimeCode::SetSeconds(unsigned int seconds) {
    if (seconds >= 60) {
        throw invalid_argument("Seconds have to be LESS than 60!");
    }
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    t = ComponentsToSeconds(hr, min, seconds);
}

// Resets the time to zero.
void TimeCode::reset() {
    t = 0;
}

// Retrieves the hour component from the total seconds.
unsigned int TimeCode::GetHours() const {
    return t / 3600;
}

// Retrieves the minute component from the total seconds.
unsigned int TimeCode::GetMinutes() const {
    return (t % 3600) / 60;
}

// Retrieves the second component from the total seconds.
unsigned int TimeCode::GetSeconds() const {
    return t % 60;
}

// Converts the time into a human-readable string (e.g., "3:15:42").
string TimeCode::ToString() const {
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    string result = "";
    result += to_string(hr) + ":";
    result += to_string(min) + ":";
    result += to_string(sec);
    return result;
}

// Adds two TimeCode objects together and returns a new TimeCode object.
TimeCode TimeCode::operator+(const TimeCode& other) const {
    return TimeCode(0, 0, t + other.t);
}

// Subtracts one TimeCode from another, ensuring the result is not negative.
TimeCode TimeCode::operator-(const TimeCode& other) const {
    if (t < other.t) {
        throw invalid_argument("Cannot have negative input!");
    }
    return TimeCode(0, 0, t - other.t);
}

// Multiplies the TimeCode by a single numeric value, useful for scaling time values.
TimeCode TimeCode::operator*(double a) const {
    return TimeCode(0, 0, static_cast<long long unsigned int>(t * a));
}

// Divides the TimeCode by a single numeric value, ensuring no division by zero occurs.
TimeCode TimeCode::operator/(double a) const {
    if (a == 0) {
        throw invalid_argument("Cannot divide by 0!");
    }
    return TimeCode(0, 0, static_cast<long long unsigned int>(t / a));
}

// Equality operator: Returns true if two TimeCode objects have the same total time.
bool TimeCode::operator==(const TimeCode& other) const {
    return t == other.t;
}

// Inequality operator: Returns true if two TimeCode objects do not have the same time.
bool TimeCode::operator!=(const TimeCode& other) const {
    return t != other.t;
}

// Less-than operator: Compares based on total time in seconds.
bool TimeCode::operator<(const TimeCode& other) const {
    return t < other.t;
}

// Less-than or equal-to operator.
bool TimeCode::operator<=(const TimeCode& other) const {
    return t <= other.t;
}

// Greater-than operator.
bool TimeCode::operator>(const TimeCode& other) const {
    return t > other.t;
}

// Greater-than or equal-to operator.
bool TimeCode::operator>=(const TimeCode& other) const {
    return t >= other.t;
}

// used zybook chapters for help