/*
 * Assignment 2: Quick Sort Implementation for Movie Sorting
 *
 * This program reads movie data from a CSV file, sorts the movies based on user-selected
 * attributes (rating, year, or views) in ascending or descending order using the quick sort
 * algorithm, and displays the top 10 sorted movies. It also measures and reports the sorting
 * time using high-resolution timing.
 *
 * Key concepts: Quick sort, file I/O, CSV parsing, user input handling, time measurement.
 */

#include <iostream>     // For input/output operations (cin, cout, cerr)
#include <vector>       // For dynamic arrays (vector)
#include <string>       // For string handling
#include <fstream>      // For file input operations (ifstream)
#include <sstream>      // For string stream parsing (stringstream)
#include <chrono>       // For high-resolution timing (high_resolution_clock, duration)
#include <algorithm>    // For string transformations (transform, ::tolower)
#include <cmath>        // For mathematical functions (though not heavily used here)

using namespace std;           // Use the standard namespace
using namespace std::chrono;   // Use chrono namespace for timing

// Structure to represent a movie with its attributes
struct Movie {
    string title;       // Title of the movie
    float rating;       // Rating score (e.g., IMDb rating)
    int releaseYear;    // Year the movie was released
    int views;          // Number of views or popularity metric

    // Method to display the movie's details in a formatted way
    void display() const {
        cout << title << " | Score: " << rating
             << " | Year: " << releaseYear
             << " | Popularity: " << views << endl;
    }
};

// Function to perform quick sort on a vector of movies using a comparison function
// Parameters:
// - data: Reference to the vector of movies to sort
// - low: Starting index of the subarray to sort
// - high: Ending index of the subarray to sort
// - compare: Pointer to a comparison function that defines the sorting order
void quickSorter(vector<Movie> &data, int low, int high, bool (*compare)(const Movie &, const Movie &)) {
    // Base case: if the subarray has one or zero elements, it's already sorted
    if (low >= high) return;

    // Choose the last element as the pivot
    Movie pivotElement = data[high];
    int pivotPos = low;  // Position where the pivot will be placed

    // Partition the array around the pivot
    for (int j = low; j < high; j++) {
        // If the current element should come before the pivot according to the compare function
        if (compare(data[j], pivotElement)) {
            swap(data[j], data[pivotPos]);  // Swap to move smaller elements to the left
            pivotPos++;  // Increment pivot position
        }
    }

    // Place the pivot in its correct position
    swap(data[pivotPos], data[high]);

    // Recursively sort the left and right subarrays
    quickSorter(data, low, pivotPos - 1, compare);
    quickSorter(data, pivotPos + 1, high, compare);
}

// Comparison functions for sorting movies in ascending and descending order by different attributes
// These functions are used as function pointers in the quick sort algorithm

// Ascending order by rating
bool byRatingAsc(const Movie &x, const Movie &y) { return x.rating < y.rating; }

// Descending order by rating
bool byRatingDesc(const Movie &x, const Movie &y) { return x.rating > y.rating; }

// Ascending order by release year
bool byYearAsc(const Movie &x, const Movie &y) { return x.releaseYear < y.releaseYear; }

// Descending order by release year
bool byYearDesc(const Movie &x, const Movie &y) { return x.releaseYear > y.releaseYear; }

// Ascending order by views
bool byViewsAsc(const Movie &x, const Movie &y) { return x.views < y.views; }

// Descending order by views
bool byViewsDesc(const Movie &x, const Movie &y) { return x.views > y.views; }

// Function to read movie data from a CSV file and return a vector of Movie objects
// Parameters:
// - path: The file path to the CSV file
// Returns: A vector of Movie objects parsed from the file
vector<Movie> readMovieCSV(const string &path) {
    vector<Movie> movieList;  // Vector to store the parsed movies
    ifstream input(path);     // Open the file for reading

    // Check if the file was opened successfully
    if (!input.is_open()) {
        cerr << "Error: Unable to open file " << path << endl;
        return movieList;  // Return empty vector on error
    }

    string line;
    getline(input, line);  // Skip the header row

    // Read each line from the file
    while (getline(input, line)) {
        stringstream ss(line);  // Use stringstream to parse the line
        string titleStr, ratingStr, yearStr, viewsStr;

        // Extract each field separated by commas
        if (!getline(ss, titleStr, ',') ||
            !getline(ss, ratingStr, ',') ||
            !getline(ss, yearStr, ',') ||
            !getline(ss, viewsStr, ',')) {
            continue;  // Skip malformed lines
        }

        // Try to parse the fields into appropriate data types
        try {
            Movie m;
            m.title = titleStr;
            m.rating = stof(ratingStr);      // Convert string to float
            m.releaseYear = stoi(yearStr);   // Convert string to int
            m.views = stoi(viewsStr);        // Convert string to int
            movieList.push_back(m);          // Add the movie to the list
        } catch (...) {
            continue;  // Skip lines with invalid data
        }
    }
    return movieList;  // Return the list of movies
}

// Main function: Entry point of the program
int main() {
    // Define the path to the CSV file containing movie data
    string filePath = "movies_real_titles.csv";

    // Read the movie data from the CSV file
    vector<Movie> movies = readMovieCSV(filePath);

    // Check if any movies were loaded
    if (movies.empty()) {
        cout << "No movie records found!\n";
        return 0;  // Exit if no data
    }

    // Prompt user for the attribute to sort by
    cout << "Sort by which attribute? (rating/year/views): ";
    string userChoice;
    cin >> userChoice;
    // Convert input to lowercase for case-insensitive comparison
    transform(userChoice.begin(), userChoice.end(), userChoice.begin(), ::tolower);

    // Prompt user for the sort order
    cout << "Sort order? (asc/desc): ";
    string orderChoice;
    cin >> orderChoice;
    // Convert input to lowercase
    transform(orderChoice.begin(), orderChoice.end(), orderChoice.begin(), ::tolower);

    // Pointer to the comparison function based on user choices
    bool (*cmp)(const Movie &, const Movie &);

    // Select the appropriate comparison function
    if (userChoice == "rating") {
        cmp = (orderChoice == "desc") ? byRatingDesc : byRatingAsc;
    } else if (userChoice == "year") {
        cmp = (orderChoice == "desc") ? byYearDesc : byYearAsc;
    } else if (userChoice == "views") {
        cmp = (orderChoice == "desc") ? byViewsDesc : byViewsAsc;
    } else {
        cout << "Invalid input! Default sorting by rating ascending.\n";
        cmp = byRatingAsc;  // Default to ascending rating
    }

    // Record the start time for performance measurement
    auto begin = high_resolution_clock::now();

    // Perform quick sort on the movies vector
    quickSorter(movies, 0, movies.size() - 1, cmp);

    // Record the end time
    auto end = high_resolution_clock::now();

    // Display the top 10 movies (assuming descending order for "top")
    cout << "\n--- Top 10 Movies Sorted by " << userChoice
         << " (" << orderChoice << ") ---\n";

    // Calculate the starting index for the last 10 movies
    int lastIdx = max(0, (int)movies.size() - 10);
    for (int i = movies.size() - 1; i >= lastIdx; --i) {
        movies[i].display();  // Display each movie
    }

    // Calculate and display the total time taken
    duration<double> totalTime = end - begin;
    cout << "\nProcessed " << movies.size() << " entries in "
         << totalTime.count() << " seconds.\n";

    return 0;  // Successful execution
}
