#include "TimeCode.h"
#include <iomanip>  // For formatting output
#include <stdexcept> // For handling exceptions
#include <sstream>   // For string stream operations

using namespace std;

// Constructor: Converts hours, minutes, and seconds into total seconds.
// Ensures proper carryover when values exceed 59.
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec) {
    min += sec / 60;  // Carry over excess seconds into minutes
    sec = sec % 60;   // Keep seconds within 0-59 range
    hr += min / 60;   // Carry over excess minutes into hours
    min = min % 60;   // Keep minutes within 0-59 range

    t = static_cast<long long unsigned int>(hr) * 3600 + 
        static_cast<long long unsigned int>(min) * 60 + sec;
}

// Copy Constructor: Creates a new TimeCode object with the same total seconds.
TimeCode::TimeCode(const TimeCode& tc) {
    t = tc.t;
}

// Converts hours, minutes, and seconds into total seconds.
// Ensures that minutes and seconds are valid (less than 60).
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec) {
    if (min >= 60 || sec >= 60) {
        throw invalid_argument("Seconds and minutes must be less than 60!");
    }
    return hr * 3600 + min * 60 + sec;
}

// Extracts hours, minutes, and seconds from the total seconds.
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const {
    unsigned long long int totalSeconds = t; // Store total seconds

    hr = static_cast<unsigned int>(totalSeconds / 3600);  // Extract hours
    totalSeconds %= 3600;  // Remaining seconds after extracting hours

    min = static_cast<unsigned int>(totalSeconds / 60);  // Extract minutes
    sec = static_cast<unsigned int>(totalSeconds % 60);  // Extract remaining seconds
}

// Updates only the hours component, leaving minutes and seconds unchanged.
void TimeCode::SetHours(unsigned int hours) {
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec); // Extract current time components
    t = ComponentsToSeconds(hours, min, sec); // Recalculate total time with new hours
}

// Updates only the minutes component, ensuring it remains valid.
void TimeCode::SetMinutes(unsigned int minutes) {
    if (minutes >= 60) {
        throw invalid_argument("Minutes must be less than 60!");
    }
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec); // Extract current time components
    t = ComponentsToSeconds(hr, minutes, sec); // Recalculate total time with new minutes
}

// Updates only the seconds component, ensuring it remains valid.
void TimeCode::SetSeconds(unsigned int seconds) {
    if (seconds >= 60) {
        throw invalid_argument("Seconds must be less than 60!");
    }
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec); // Extract current time components
    t = ComponentsToSeconds(hr, min, seconds); // Recalculate total time with new seconds
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
    GetComponents(hr, min, sec); // Extract time components
    
    ostringstream oss;
    oss << hr << ":" << min << ":" << sec; // Format output as "hh:mm:ss"
    return oss.str();
}

// Adds two TimeCode objects together and returns a new TimeCode object.
TimeCode TimeCode::operator+(const TimeCode& other) const {
    return TimeCode(0, 0, t + other.t); // Add total seconds and create new object
}

// Subtracts one TimeCode from another, ensuring the result is not negative.
TimeCode TimeCode::operator-(const TimeCode& other) const {
    if (t < other.t) {
        throw invalid_argument("Cannot have negative time!");
    }
    return TimeCode(0, 0, t - other.t); // Subtract total seconds and create new object
}

// Multiplies the TimeCode by a numeric value, useful for scaling time values.
TimeCode TimeCode::operator*(double a) const {
    if (a < 0) {
        throw invalid_argument("Cannot multiply by a negative number!");
    }
    long long unsigned int newSeconds = static_cast<long long unsigned int>(t * a); // Scale time
    return TimeCode(0, 0, newSeconds);
}

// Divides the TimeCode by a numeric value, ensuring no division by zero occurs.
TimeCode TimeCode::operator/(double a) const {
    if (a == 0) {
        throw invalid_argument("Cannot divide by 0!");
    }
    if (a < 0) {
        throw invalid_argument("Cannot divide by a negative number!");
    }
    long long unsigned int newSeconds = static_cast<long long unsigned int>(t / a); // Scale time
    return TimeCode(0, 0, newSeconds);
}

// Equality operator: Returns true if two TimeCode objects have the same total time.
bool TimeCode::operator==(const TimeCode& other) const {
    return t == other.t;
}

// Inequality operator: Returns true if two TimeCode objects have different total time.
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