#include <iostream>
#include <ctime>       // For time(0)
#include <vector>      // For vector storage
#include <cmath>       // For sphere surface area calculation
#include <cstdlib>     // For rand()
#include <cassert>     // For testing
#include "TimeCode.h"  // TimeCode class

using namespace std;

// Struct to store drying batch information
struct DryingSnapShot {
    string name;         // Name of the batch
    int batchID;         // Unique batch ID (generated with rand())
    time_t startTime;    // Time when drying started
    TimeCode* timeToDry; // Pointer to heap-allocated TimeCode
};

// Function to generate a random batch ID
int generateBatchID() {
    return rand(); // Generates a random integer as batch ID
}

// Function to calculate the surface area of a sphere
// Formula: 4 * π * r^2
double get_sphere_sa(double radius) {
    return 4 * M_PI * radius * radius;
}

// Function to compute drying time based on surface area
// Uses total surface area as seconds to dry (as an arbitrary mapping)
TimeCode* compute_time_code(double surfaceArea) {
    return new TimeCode(0, 0, static_cast<unsigned long long>(surfaceArea)); // Allocate dynamically
}

// Function to calculate remaining drying time
long long int get_time_remaining(DryingSnapShot& dss) {
    time_t elapsed = time(0) - dss.startTime; // Time elapsed since start
    long long totalSeconds = dss.timeToDry->GetTimeCodeAsSeconds(); // Get total drying time
    return max(0LL, totalSeconds - elapsed); // Ensure time remaining is non-negative
}

// Function to format DryingSnapShot for printing
string drying_snap_shot_to_string(DryingSnapShot& dss) {
    long long remaining = get_time_remaining(dss); // Get remaining time in seconds
    
    // Convert remaining seconds into hours, minutes, and seconds
    TimeCode remainingTime(
        remaining / 3600,         // Hours
        (remaining % 3600) / 60,  // Minutes
        remaining % 60            // Seconds
    );

    if (remaining > 0) {
        return "Batch-" + to_string(dss.batchID) + " (" + dss.name + ") drying. Time remaining: " + remainingTime.ToString();
    } else {
        return "Batch-" + to_string(dss.batchID) + " (" + dss.name + ") has finished drying!";
    }
}

int main() {
    srand(time(0)); // Seed random number generator

    vector<DryingSnapShot> dryingBatches;  // Store all drying batches
    char choice;

    while (true) {
        cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
        cin >> choice;
        choice = tolower(choice);

        if (choice == 'a') { // Add a new drying batch
            DryingSnapShot dss;
            cout << "Enter batch name: ";
            cin >> dss.name;

            double radius;
            cout << "Enter radius of each object in cm: ";
            cin >> radius;

            dss.batchID = generateBatchID(); // Assign a random batch ID
            double surfaceArea = get_sphere_sa(radius); // Calculate surface area
            dss.startTime = time(0); // Record current time as start time
            dss.timeToDry = compute_time_code(surfaceArea); // Allocate drying time dynamically

            cout << "Batch-" << dss.batchID << " (" << dss.name << ") is now drying." << endl;
            dryingBatches.push_back(dss); // Store drying batch in vector
        }
        else if (choice == 'v') { // View drying items
            if (dryingBatches.empty()) {
                cout << "No drying batches being tracked." << endl;
            } else {
                auto it = dryingBatches.begin();
                while (it != dryingBatches.end()) {
                    cout << drying_snap_shot_to_string(*it) << endl;

                    if (get_time_remaining(*it) == 0) {
                        // Free memory and remove from list if drying is complete
                        delete it->timeToDry;
                        it = dryingBatches.erase(it);  // Remove from vector
                    } else {
                        ++it;
                    }
                }
                cout << dryingBatches.size() << " batches being tracked." << endl;
            }
        }
        else if (choice == 'q') { // Quit program
            cout << "Exiting and freeing memory..." << endl;
            for (auto& dss : dryingBatches) {
                delete dss.timeToDry; // Free remaining memory
            }
            dryingBatches.clear(); // Clear all batches from vector
            break;
        }
        else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}