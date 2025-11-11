/*
 * Assignment 7: Graph Coloring for Exam Scheduling
 *
 * This program models exam scheduling as a graph coloring problem. Courses are nodes,
 * and conflicts (students enrolled in multiple courses) are edges. It uses the Welsh-Powell
 * heuristic to color the graph, assigning time slots to exams, and then assigns rooms
 * based on capacity and student enrollment.
 *
 * Key concepts: Graph coloring, Welsh-Powell algorithm, conflict graph, room allocation.
 */

#include <iostream>        // For input/output operations (cin, cout, ios::sync_with_stdio, cin.tie)
#include <vector>          // For dynamic arrays (vector)
#include <string>          // For string handling (string, getline)
#include <unordered_map>   // For mapping course names to indices (unordered_map)
#include <unordered_set>   // For tracking occupied rooms (unordered_set)
#include <algorithm>       // For sorting and algorithms (sort, max, unique)
#include <numeric>         // For generating sequences (iota)
#include <iomanip>         // For output formatting (setw, though not used in final output)

using namespace std;  // Use the standard namespace

// Main function: Entry point of the program
int main() {
    // Optimize C++ stream I/O for speed (disable synchronization with C stdio)
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- 1. Read Courses ---
    int numCourses;
    if (!(cin >> numCourses)) return 0;  // Exit if input fails

    // Vectors and maps for course data
    vector<string> courseNames(numCourses);  // List of course names
    unordered_map<string, int> courseIndex;  // Map course name to index
    for (int i = 0; i < numCourses; i++) {
        cin >> ws;  // Consume trailing newline or whitespace
        getline(cin, courseNames[i]);  // Read the full course name
        courseIndex[courseNames[i]] = i;  // Map name to index
    }

    // --- 2. Build Conflict Graph ---
    int numStudents;
    cin >> numStudents;

    // Vectors for graph representation
    vector<int> studentsIn(numCourses, 0);  // Count of students enrolled in each course
    vector<vector<int>> adj(numCourses);    // Adjacency list for conflict graph

    // Process each student
    for (int si = 0; si < numStudents; si++) {
        string sid;  // Student ID (not used further)
        int k;       // Number of courses this student is enrolled in
        cin >> ws;
        cin >> sid >> k;

        vector<int> enrolled;  // List of course indices this student is enrolled in
        for (int j = 0; j < k; j++) {
            string cname;  // Course name
            cin >> ws;
            getline(cin, cname);
            if (courseIndex.find(cname) == courseIndex.end()) continue;  // Skip invalid courses

            int idx = courseIndex[cname];  // Get course index
            enrolled.push_back(idx);
            studentsIn[idx]++;  // Increment student count for this course
        }

        // Add edges between all pairs of courses this student is enrolled in (conflicts)
        sort(enrolled.begin(), enrolled.end());  // Sort to remove duplicates
        enrolled.erase(unique(enrolled.begin(), enrolled.end()), enrolled.end());

        // Create undirected edges for conflicts
        for (size_t a = 0; a < enrolled.size(); a++) {
            for (size_t b = a + 1; b < enrolled.size(); b++) {
                int u = enrolled[a], v = enrolled[b];
                adj[u].push_back(v);  // Add edge u-v
                adj[v].push_back(u);  // Add edge v-u
            }
        }
    }

    // Clean up adjacency lists: sort and remove duplicate edges, compute degrees
    vector<int> degree(numCourses);
    for (int i = 0; i < numCourses; i++) {
        sort(adj[i].begin(), adj[i].end());  // Sort adjacency list
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());  // Remove duplicates
        degree[i] = adj[i].size();  // Degree of node i
    }

    // --- 3. Graph Coloring (Welsh-Powell Heuristic) ---
    // Create an 'order' vector [0, 1, 2, ..., C-1] and sort by decreasing degree
    vector<int> order(numCourses);
    iota(order.begin(), order.end(), 0);  // Fill with 0 to numCourses-1

    // Sort order by decreasing degree (highest degree first), then by index
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (degree[a] != degree[b]) return degree[a] > degree[b];
        return a < b;
    });

    // Color assignment: -1 means uncolored
    vector<int> color(numCourses, -1);
    int maxColor = 0;  // Track the maximum color used

    // Assign colors using Welsh-Powell heuristic
    for (int idx : order) {
        vector<char> used(numCourses + 1, false);  // Tracks colors used by neighbors

        // Mark colors used by already colored neighbors
        for (int v : adj[idx]) {
            if (color[v] != -1) {
                used[color[v]] = true;
            }
        }

        // Find the smallest available color (1-indexed)
        int c = 1;
        while (c < used.size() && used[c]) c++;

        color[idx] = c;  // Assign the color
        maxColor = max(maxColor, c);  // Update max color
    }

    // --- 4. Room Allocation ---
    int numRooms;
    cin >> numRooms;

    // Read room details: name and capacity
    vector<pair<string, int>> rooms(numRooms);
    for (int i = 0; i < numRooms; i++) {
        string rname;
        int cap;
        cin >> ws;
        getline(cin, rname);  // Read room name
        cin >> cap;           // Read capacity
        rooms[i] = {rname, cap};
    }

    // Sort rooms by capacity ascending (smallest first for best-fit)
    vector<int> roomOrder(numRooms);
    iota(roomOrder.begin(), roomOrder.end(), 0);  // Fill with 0 to numRooms-1
    sort(roomOrder.begin(), roomOrder.end(), [&](int a, int b) {
        if (rooms[a].second != rooms[b].second) {
            return rooms[a].second < rooms[b].second;  // Ascending capacity
        }
        return a < b;  // Stable sort by index
    });

    // Track room occupancy per time slot
    vector<unordered_set<int>> roomOccupied(maxColor + 1);  // Rooms used in each slot
    vector<string> assignedRoom(numCourses, "Unassigned");   // Assigned room for each course

    // Assign rooms for each time slot (color)
    for (int t = 1; t <= maxColor; t++) {
        vector<int> exams;  // Courses in this time slot
        for (int i = 0; i < numCourses; i++) {
            if (color[i] == t) exams.push_back(i);
        }

        // Sort exams by student count descending (largest first)
        sort(exams.begin(), exams.end(), [&](int a, int b) {
            if (studentsIn[a] != studentsIn[b]) {
                return studentsIn[a] > studentsIn[b];  // Descending student count
            }
            return a < b;  // Stable sort by index
        });

        // Assign rooms using best-fit approach
        for (int e : exams) {
            for (int ri : roomOrder) {
                // Check if room has enough capacity and is not occupied in this slot
                if (rooms[ri].second >= studentsIn[e] &&
                    roomOccupied[t].count(ri) == 0) {

                    assignedRoom[e] = rooms[ri].first;  // Assign room
                    roomOccupied[t].insert(ri);         // Mark room as occupied
                    break;  // Move to next exam
                }
            }
        }
    }

    // --- 5. Display Final Timetable ---
    // Output the schedule in tabular format
    cout << "Course\tTimeSlot\tRoom\n";
    for (int i = 0; i < numCourses; i++) {
        cout << courseNames[i] << "\tSlot " << color[i] << "\t" << assignedRoom[i] << "\n";
    }

    return 0;  // Successful execution
}
