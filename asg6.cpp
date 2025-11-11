/*
 * Assignment 6: 0/1 Knapsack Problem for Truck Loading
 *
 * This program solves the 0/1 knapsack problem to maximize the utility value of items loaded
 * into a truck with a weight capacity. It uses dynamic programming to compute the optimal
 * selection of items and displays the selected items with their values and weights.
 *
 * Key concepts: Dynamic programming, 0/1 knapsack, backtracking for solution reconstruction.
 */

#include <iostream>    // For input/output operations (cin, cout)
#include <vector>      // For dynamic arrays (vector)
#include <string>      // For string handling
#include <iomanip>     // For output formatting (though not heavily used here)

using namespace std;  // Use the standard namespace

// Structure to represent an item with its properties
struct Item {
    string name;   // Name of the item
    int value;     // Utility value of the item
    int weight;    // Weight of the item
};

// Function to solve the 0/1 knapsack problem using dynamic programming
// Parameters:
// - W: Maximum weight capacity of the truck
// - items: Vector of available items
void solveKnapsack(int W, const vector<Item>& items) {
    int n = items.size();  // Number of items

    // DP table: dp[i][w] = maximum value using first i items with weight limit w
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Fill the DP table
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= W; ++w) {
            // If the current item's weight is less than or equal to current capacity
            if (items[i - 1].weight <= w) {
                // Choose max of: including the item or not including it
                dp[i][w] = max(items[i - 1].value + dp[i - 1][w - items[i - 1].weight],
                               dp[i - 1][w]);
            } else {
                // Cannot include the item, take the value without it
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Output the optimal solution
    cout << "\nOptimal Solution:\n";
    cout << "Maximum Utility Value: " << dp[n][W] << endl;
    cout << "Items to include in the truck:\n";

    // Backtrack to find which items were included
    int maxValue = dp[n][W];  // Current maximum value
    int w = W;                // Current weight capacity
    for (int i = n; i > 0 && maxValue > 0; --i) {
        // If the value changed, this item was included
        if (maxValue != dp[i - 1][w]) {
            // This item was included
            cout << "- " << items[i - 1].name << " (Value: "
                 << items[i - 1].value << ", Weight: "
                 << items[i - 1].weight << ")" << endl;

            // Update remaining value and weight
            maxValue -= items[i - 1].value;
            w -= items[i - 1].weight;
        }
    }
    cout << "----------------------------------------\n";
}

// Main function: Entry point of the program
int main() {
    // Variable for truck's weight capacity
    int truckCapacity;
    cout << "Enter the truck's maximum weight capacity: ";
    cin >> truckCapacity;

    // Number of item types
    int n;
    cout << "Enter the number of available item types: ";
    cin >> n;

    // Vector to store the items
    vector<Item> availableItems(n);
    cout << "Enter the details for each item (name value weight):" << endl;
    for (int i = 0; i < n; ++i) {
        // Read item details from input
        cin >> availableItems[i].name >> availableItems[i].value >>
               availableItems[i].weight;
    }

    // Solve the knapsack problem and display results
    solveKnapsack(truckCapacity, availableItems);
    return 0;  // Successful execution
}
