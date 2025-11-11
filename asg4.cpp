/*
 * Assignment 4: Dijkstra's Algorithm for Ambulance Routing
 *
 * This program implements Dijkstra's algorithm to find the shortest path from an ambulance
 * start location to the nearest hospital in a graph representing city intersections and roads.
 * It allows updating travel times due to traffic and finds the optimal route with the minimum
 * estimated time.
 *
 * Key concepts: Dijkstra's algorithm, shortest path, priority queue, graph representation.
 */

#include <iostream>     // For input/output operations (cin, cout)
#include <vector>       // For dynamic arrays (vector)
#include <queue>        // For priority queue
#include <limits>       // For numeric limits (numeric_limits)
#include <algorithm>    // For algorithms (though not heavily used here)

using namespace std;  // Use the standard namespace

// Define infinity as the maximum int value
const int INT_MAX = numeric_limits<int>::max();

// Type alias for a pair of integers (used for edges: {vertex, weight})
typedef pair<int, int> iPair;

// Function to compute shortest paths from source using Dijkstra's algorithm
// Parameters:
// - src: Source vertex (starting point)
// - adj: Adjacency list representing the graph (vector of vectors of pairs)
// - dist: Vector to store shortest distances from source
// - parent: Vector to store parent nodes for path reconstruction
void dijkstra(int src, const vector<vector<iPair>>& adj, vector<int>& dist, vector<int>& parent) {
    // Number of vertices in the graph
    int V = adj.size();

    // Initialize distances to infinity and parents to -1
    dist.assign(V, INT_MAX);
    parent.assign(V, -1);

    // Priority queue to select the next vertex with minimum distance (min-heap)
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

    // Distance to source is 0
    dist[src] = 0;
    pq.push({0, src});  // Push {distance, vertex}

    // Main loop: process vertices in order of increasing distance
    while (!pq.empty()) {
        // Extract the vertex with the smallest distance
        int u = pq.top().second;  // Vertex
        int d = pq.top().first;   // Distance
        pq.pop();

        // Skip if a better distance was already found (outdated entry)
        if (d > dist[u]) continue;

        // Relax all adjacent edges
        for (auto& edge : adj[u]) {
            int v = edge.first;   // Adjacent vertex
            int w = edge.second;  // Edge weight

            // If a shorter path to v is found through u
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;  // Update distance
                parent[v] = u;          // Update parent
                pq.push({dist[v], v});  // Push updated distance
            }
        }
    }
}

// Function to reconstruct and print the path from source to a node using parent array
// Parameters:
// - node: The destination node
// - parent: Vector containing parent nodes for each vertex
void printPath(int node, const vector<int>& parent) {
    // Vector to store the path nodes
    vector<int> path;

    // Traverse from node to source using parent pointers
    while (node != -1) {
        path.push_back(node);
        node = parent[node];
    }

    // Reverse the path to get it from source to destination
    reverse(path.begin(), path.end());

    // Print the path
    cout << "Optimal Path: ";
    for (int i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Main function: Entry point of the program
int main() {
    // Variables for number of vertices (intersections) and edges (roads)
    int V, E;
    cout << "Enter number of intersections (vertices): ";
    cin >> V;
    cout << "Enter number of roads (edges): ";
    cin >> E;

    // Adjacency list to represent the graph (undirected)
    vector<vector<iPair>> graph(V);
    cout << "Enter roads (u v travel time):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});  // Add edge u -> v
        graph[v].push_back({u, w});  // Add edge v -> u (undirected)
    }

    // Option to update travel times due to traffic
    char update;
    cout << "Do you want to update travel times due to traffic? (y/n): ";
    cin >> update;
    while (update == 'y') {
        int u, v, w;
        cout << "Enter road to update (u v new_travel time): ";
        cin >> u >> v >> w;

        // Update the edge u -> v
        for (auto& edge : graph[u]) {
            if (edge.first == v) edge.second = w;
        }
        // Update the edge v -> u
        for (auto& edge : graph[v]) {
            if (edge.first == u) edge.second = w;
        }

        cout << "Update another road? (y/n): ";
        cin >> update;
    }

    // Ambulance start location
    int source;
    cout << "Enter ambulance start location (source): ";
    cin >> source;

    // Number of hospitals and their locations
    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital node indices: ";
    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    // Vectors to store distances and parents from Dijkstra's
    vector<int> dist, parent;
    dijkstra(source, graph, dist, parent);

    // Find the nearest hospital
    int nearestHospital = -1, minTime = INT_MAX;
    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    // Output the result
    if (nearestHospital == -1 || dist[nearestHospital] == INT_MAX) {
        cout << "No hospital reachable.\n";
    } else {
        cout << "\nNearest hospital is at node " << nearestHospital
             << " with estimated time " << minTime << " minutes.\n";
        printPath(nearestHospital, parent);  // Print the path
    }

    return 0;  // Successful execution
}
