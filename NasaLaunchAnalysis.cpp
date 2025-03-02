#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "TimeCode.h"

using namespace std;

/**
 * Splits a CSV line into fields, handling quoted values correctly.
 * @param line The CSV line to split.
 * @return A vector of parsed fields.
 */
vector<string> split_csv(const string &line) {
    vector<string> result;
    string field;
    bool inside_quotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inside_quotes = !inside_quotes;  // Toggle quote tracking
        } else if (ch == ',' && !inside_quotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += ch;
        }
    }

    result.push_back(field);  // Add last field
    return result;
}

/**
 * Extracts and parses the time (HH:MM) from the "Datum" column.
 * @param line The CSV line containing the timestamp.
 * @return A TimeCode object representing the extracted time.
 */
TimeCode parse_line(const string &line) {
    vector<string> fields = split_csv(line);

    if (fields.size() <= 3) {
        return TimeCode(-1, -1, -1);  // Return an invalid marker
    }

    string datum = fields[3];
    

    size_t utc_pos = datum.rfind(" UTC");
    if (utc_pos == string::npos) {
        return TimeCode(-1, -1, -1);
    }

    size_t time_start = datum.rfind(" ", utc_pos - 1);
    if (time_start == string::npos) {
        return TimeCode(-1, -1, -1);
    }

    string time_part = datum.substr(time_start + 1, utc_pos - time_start - 1);

    // Validate and extract hours/minutes
    istringstream iss(time_part);
    unsigned int hours, minutes;
    char sep;
    if (!(iss >> hours >> sep >> minutes) || sep != ':') {
        return TimeCode(-1, -1, -1);
    }

  
   

    return TimeCode(hours, minutes, 0);
}

/**
 * Main function that reads a CSV file, extracts launch times, 
 * calculates the average time, and outputs the results.
 */
int main() {
    ifstream file("Space_Corrected.csv");
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    vector<TimeCode> launch_times;
    string line;
    int skipped_rows = 0;  // Counter for skipped rows

    getline(file, line);  // Skip header row

    while (getline(file, line)) {
        TimeCode time = parse_line(line);
        if (time.GetHours() >= 0 && time.GetHours() < 24 && time.GetMinutes() < 60) { 
            launch_times.push_back(time);
        } else {
            skipped_rows++;  // Increment count if row is skipped
        }
    }

    file.close();

   

    if (launch_times.empty()) {
        cout << "No valid time data found." << endl;
        return 1;
    }

    // Compute the average time
    TimeCode sum_time;
    for (const auto &time : launch_times) {
        sum_time = sum_time + time;
    }

    TimeCode avg_time = sum_time / static_cast<double>(launch_times.size());

    cout << launch_times.size() << " data points." << endl;
    cout << "AVERAGE: " << avg_time.ToString() << endl;

    return 0;
}
