/*
 * Assignment 5: Dynamic Programming for Delivery Route Optimization
 *
 * This program uses dynamic programming to find the optimal delivery route with minimum cost
 * in a graph of nodes (cities) and edges (routes with costs). It computes the minimum cost path
 * from the first node to the last node and reconstructs the optimal route.
 *
 * Key concepts: Dynamic programming, shortest path, path reconstruction.
 */

#include <iostream>     // For input/output operations (cin, cout)
#include <vector>       // For dynamic arrays (vector)
#include <limits>       // For numeric limits (numeric_limits)
#include <algorithm>    // For algorithms (though not heavily used here)

using namespace std;  // Use the standard namespace

// Define infinity as the maximum int value
const int INF = numeric_limits<int>::max();

// Function to find the optimal route using dynamic programming
// Parameters:
// - N: Number of nodes (cities)
// - costMatrix: 2D vector representing the cost between nodes (INF if no direct route)
void findOptimalRoute(int N, const vector<vector<int>>& costMatrix) {
    // DP array to store minimum cost to reach each node from the end
    vector<int> dp(N, 0);
    // Path array to store the next node in the optimal path
    vector<int> path(N, 0);

    // Fill DP table from the second last node to the first
    for (int i = N - 2; i >= 0; --i) {
        dp[i] = INF;  // Initialize to infinity
        for (int j = i + 1; j < N; ++j) {
            // If there is a direct route from i to j
            if (costMatrix[i][j] != INF) {
                // If dp[j] is not infinity (reachable)
                if (dp[j] != INF) {
                    int cost = costMatrix[i][j] + dp[j];  // Total cost to go i -> j -> end
                    if (cost < dp[i]) {
                        dp[i] = cost;  // Update minimum cost
                        path[i] = j;   // Record the next node
                    }
                }
            }
        }
    }

    // Output the results
    cout << "\n----------------------------------------\n";
    if (dp[0] == INF) {
        cout << "No route found from source to destination." << endl;
    } else {
        cout << "Minimum Delivery Cost: " << dp[0] << endl;
        cout << "Optimal Route Path: ";

        // Reconstruct the path from source (0) to destination (N-1)
        int currentNode = 0;
        while (currentNode != N - 1 && path[currentNode] != 0) {
            cout << currentNode << " -> ";
            currentNode = path[currentNode];
        }
        cout << N - 1 << endl;
    }
    cout << "----------------------------------------\n";
}

// Main function: Entry point of the program
int main() {
    // Variables for number of nodes and edges
    int N, E;
    cout << "Enter the total number of nodes: ";
    cin >> N;
    cout << "Enter the total number of routes (edges): ";
    cin >> E;

    // Initialize cost matrix with INF (no direct routes)
    vector<vector<int>> costMatrix(N, vector<int>(N, INF));

    // Read edge details and populate the cost matrix
    cout << "Enter details for each route (source destination cost):" << endl;
    for (int i = 0; i < E; ++i) {
        int u, v, cost;
        cin >> u >> v >> cost;
        // Validate indices and set the cost
        if (u >= 0 && u < N && v >= 0 && v < N) {
            costMatrix[u][v] = cost;
        }
    }

    // Compute and display the optimal route
    findOptimalRoute(N, costMatrix);
    return 0;  // Successful execution
}
