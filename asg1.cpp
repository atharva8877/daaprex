/*
 * Assignment 1: Merge Sort Implementation for Order Sorting
 *
 * This program demonstrates the merge sort algorithm applied to sorting orders by timestamp.
 * It generates random orders with timestamps, sorts them using merge sort, and tracks the
 * auxiliary space used during the sorting process. The program also measures the time taken
 * for sorting and displays the first 5 sorted orders.
 *
 * Key concepts: Merge sort, time complexity (O(n log n)), space complexity analysis,
 * timestamp handling in C++.
 */

#include <iostream>  // For input/output operations (cin, cout)
#include <vector>    // For dynamic arrays (vector)
#include <ctime>     // For time-related functions (time_t, mktime, gmtime, strftime)
#include <cstdlib>   // For random number generation (rand, srand)
#include <iomanip>   // For output formatting (fixed, setprecision, setw)

using namespace std;  // Use the standard namespace to avoid prefixing std::

// Global variable to track the maximum auxiliary space used during merge operations
size_t max_auxiliary_space = 0;

// Structure to represent an order with an ID and a timestamp
struct Order {
    int order_id;      // Unique identifier for the order
    time_t timestamp;  // Timestamp when the order was placed (Unix timestamp)
};

// Function to merge two sorted subarrays of orders into a single sorted array
// Parameters:
// - orders: The vector of orders to be sorted
// - left: Starting index of the left subarray
// - mid: Ending index of the left subarray (mid + 1 is start of right)
// - right: Ending index of the right subarray
void merge(vector<Order>& orders, int left, int mid, int right) {
    // Calculate sizes of the two subarrays
    int n1 = mid - left + 1;  // Size of left subarray
    int n2 = right - mid;     // Size of right subarray

    // Create temporary vectors for the left and right subarrays
    vector<Order> L(orders.begin() + left, orders.begin() + mid + 1);
    vector<Order> R(orders.begin() + mid + 1, orders.begin() + right + 1);

    // Track auxiliary space usage: space for L and R vectors
    size_t current_aux_space = (L.size() + R.size()) * sizeof(Order);
    if (current_aux_space > max_auxiliary_space) {
        max_auxiliary_space = current_aux_space;  // Update max auxiliary space if larger
    }

    // Merge the two subarrays back into the original array
    int i = 0, j = 0, k = left;  // i: index for L, j: index for R, k: index for orders
    while (i < n1 && j < n2) {
        // Compare timestamps and place the smaller one into orders
        if (L[i].timestamp <= R[j].timestamp) {
            orders[k++] = L[i++];
        } else {
            orders[k++] = R[j++];
        }
    }
    // Copy any remaining elements from L
    while (i < n1) {
        orders[k++] = L[i++];
    }
    // Copy any remaining elements from R
    while (j < n2) {
        orders[k++] = R[j++];
    }
}

// Recursive function to perform merge sort on the orders array
// Parameters:
// - orders: The vector of orders to be sorted
// - left: Starting index of the subarray to sort
// - right: Ending index of the subarray to sort
void merge_sort(vector<Order>& orders, int left, int right) {
    // Base case: if the subarray has one or zero elements, it's already sorted
    if (left < right) {
        // Calculate the middle index to divide the array into two halves
        int mid = left + (right - left) / 2;

        // Recursively sort the left half
        merge_sort(orders, left, mid);

        // Recursively sort the right half
        merge_sort(orders, mid + 1, right);

        // Merge the two sorted halves
        merge(orders, left, mid, right);
    }
}

// Function to generate a vector of sample orders with random timestamps
// Parameters:
// - orders: Reference to the vector where orders will be stored
// - n: Number of orders to generate
void generate_sample_orders(vector<Order>& orders, int n) {
    // Set up a base time structure for June 24, 2025, at 12:00 PM
    struct tm base_time_tm = {};
    base_time_tm.tm_year = 2025 - 1900;  // tm_year is years since 1900
    base_time_tm.tm_mon = 5;             // tm_mon is 0-based (5 = June)
    base_time_tm.tm_mday = 24;           // Day of the month
    base_time_tm.tm_hour = 12;           // Hour (12 PM)

    // Convert the tm structure to time_t (Unix timestamp)
    time_t base_time = mktime(&base_time_tm);

    // Resize the orders vector to hold n orders
    orders.resize(n);

    // Generate each order with a random timestamp offset
    for (int i = 0; i < n; i++) {
        // Generate a random number of minutes (0 to 99999)
        int random_minutes = rand() % 100000;

        // Add the random minutes (converted to seconds) to the base time
        orders[i].timestamp = base_time + (random_minutes * 60);

        // Assign a sequential order ID starting from 1
        orders[i].order_id = i + 1;
    }
}

// Function to print the first n sorted orders in a formatted way
// Parameters:
// - orders: The vector of sorted orders
// - n: Number of orders to print (typically 5)
void print_first_n_orders(const vector<Order>& orders, int n) {
    // Buffer to hold the formatted timestamp string
    char time_str[30];

    // Print header for the output
    cout << "\n--- First 5 Sorted Orders ---\n";

    // Determine the limit to avoid going out of bounds
    int limit = min(n, static_cast<int>(orders.size()));

    // Loop through the first 'limit' orders and print their details
    for (int i = 0; i < limit; i++) {
        // Format the timestamp as ISO 8601 string (e.g., 2025-06-24T12:00:00Z)
        strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%SZ",
            gmtime(&orders[i].timestamp));

        // Print the order ID and timestamp in a formatted table-like structure
        cout << "Order ID: " << left << setw(10) << orders[i].order_id
             << "| Timestamp: " << time_str << endl;
    }
}

// Main function: Entry point of the program
int main() {
    // Seed the random number generator with the current time for randomness
    srand(time(nullptr));

    // Variable to store the number of orders to generate
    int num_orders;

    // Prompt the user for input
    cout << "Enter the number of orders to generate and sort: ";
    cin >> num_orders;

    // Validate input: check for failure or non-positive numbers
    if (cin.fail() || num_orders <= 0) {
        cout << "Error: Invalid input. Please enter a positive number." << endl;
        return 1;  // Exit with error code
    }

    // Vector to hold the generated orders
    vector<Order> orders;

    // Generate the sample orders
    cout << "\nGenerating " << num_orders << " orders...\n";
    generate_sample_orders(orders, num_orders);

    // Start sorting and measure time
    cout << "Sorting orders using Merge Sort...\n";
    clock_t start = clock();  // Record start time

    // Perform merge sort if there are orders to sort
    if (!orders.empty()) {
        merge_sort(orders, 0, orders.size() - 1);
    }

    clock_t end = clock();  // Record end time
    cout << "Done!\n";

    // Calculate and display the time taken for sorting
    double time_taken = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "\nTime taken to sort " << num_orders << " orders: "
         << fixed << setprecision(2) << time_taken << " seconds.\n";

    // Print the first 5 sorted orders
    print_first_n_orders(orders, 5);

    // Display space complexity report
    cout << "\n--- Space Complexity Report ---\n";
    cout << "Size of one Order object: " << sizeof(Order) << " bytes\n";
    cout << "Total input size: " << orders.size() * sizeof(Order) << " bytes\n";
    cout << "Peak auxiliary space used: " << max_auxiliary_space << " bytes\n";

    return 0;  // Successful execution
}
