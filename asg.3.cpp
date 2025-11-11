/*
 * Assignment 3: Fractional Knapsack Problem with Priorities
 *
 * This program solves a variant of the fractional knapsack problem where items have
 * priorities and some are divisible. It loads items into a boat with a weight capacity,
 * prioritizing higher priority items first, then by value-to-weight ratio. Divisible items
 * can be partially loaded to fill the remaining capacity.
 *
 * Key concepts: Greedy algorithm, fractional knapsack, priority-based sorting.
 */

#include <iostream>    // For input/output operations (cin, cout)
#include <vector>      // For dynamic arrays (vector)
#include <string>      // For string handling
#include <algorithm>  // For sorting (sort)
#include <iomanip>     // For output formatting (fixed, setprecision, setw)

using namespace std;  // Use the standard namespace

// Structure to represent an item with its properties
struct Item {
    string name;      // Name of the item
    double weight;    // Weight of the item in kg
    double value;     // Utility value of the item
    int priority;     // Priority level (lower number = higher priority)
    bool divisible;   // Whether the item can be divided (true for divisible, false for indivisible)
    double ratio;     // Value-to-weight ratio (computed later)
};

// Comparison function for sorting items
// Sorts by priority first (ascending, lower priority number first), then by ratio (descending)
bool cmp(const Item& a, const Item& b) {
    // If priorities differ, sort by priority (lower priority first)
    if (a.priority != b.priority)
        return a.priority < b.priority;
    // If priorities are equal, sort by value-to-weight ratio (higher ratio first)
    return a.ratio > b.ratio;
}

// Main function: Entry point of the program
int main() {
    // Variable for boat capacity
    double W;
    cout << "Enter maximum boat capacity (kg): ";
    cin >> W;

    // Initialize a vector of items with predefined data
    vector<Item> items = {
        {"First Aid Box", 2.0, 70, 1, false},
        {"Medicine Kit", 6.0, 120, 1, false},
        {"Food Pack", 4.0, 80, 2, true},
        {"Water Bottles", 5.0, 50, 2, true},
        {"Blankets", 3.0, 40, 3, false}
    };

    // Compute the value-to-weight ratio for each item
    for (auto &item : items)
        item.ratio = item.value / item.weight;

    // Sort the items using the custom comparison function
    sort(items.begin(), items.end(), cmp);

    // Display the sorted items in a formatted table
    cout << fixed << setprecision(2);  // Set output to fixed-point notation with 2 decimals
    cout << "\nSorted Items (by priority, then value/weight):\n";
    cout << left << setw(20) << "Item"
         << setw(8) << "Weight"
         << setw(8) << "Value"
         << setw(8) << "Priority"
         << setw(12) << "Value/Weight"
         << setw(10) << "Type" << endl;
    cout << "---------------------------------------------------------------------\n";

    for (auto &item: items) {
        cout << setw(20) << item.name
             << setw(8) << item.weight
             << setw(8) << item.value
             << setw(8) << item.priority
             << setw(12) << item.ratio
             << setw(10) << (item.divisible ? "Divisible" : "Indivisible") << endl;
    }

    // Variables to track total value and current weight loaded
    double totalValue = 0;
    double currentWeight = 0;

    cout << "\n--- Loading the Boat ---\n";

    // Iterate through sorted items to load into the boat
    for (auto &item : items) {
        if (currentWeight + item.weight <= W) {
            // Take the whole item if it fits
            currentWeight += item.weight;
            totalValue += item.value;
            cout << "Loaded: " << item.name << " (Weight: " << item.weight
                 << "kg, Value: " << item.value << ")\n";
        } else if (item.divisible) {
            // Take a fraction of the divisible item to fill remaining capacity
            double remainingWeight = W - currentWeight;
            if (remainingWeight > 0) {
                double fraction = remainingWeight / item.weight;
                totalValue += item.value * fraction;
                currentWeight += remainingWeight;
                cout << "Loaded: " << (fraction * 100) << "% of " << item.name
                     << " (Weight: " << remainingWeight << "kg, Value: "
                     << (item.value * fraction) << ")\n";
                break;  // Boat is now full
            }
        } else {
            // Skip indivisible items that don't fit
            cout << "Skipped: " << item.name << " (Weight: " << item.weight
                 << "kg) - Indivisible and too heavy.\n";
        }
        if (currentWeight == W) break;  // Stop if boat is exactly full
    }

    // Display the final report
    cout << "\n--- Final Report ---\n";
    cout << "Total weight in boat: " << currentWeight << " kg\n";
    cout << "Total utility value: " << totalValue << "\n";

    return 0;  // Successful execution
}
