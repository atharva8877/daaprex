/*
 * Assignment 8: Traveling Salesman Problem using Branch and Bound
 *
 * This program solves the Traveling Salesman Problem (TSP) using the Branch and Bound
 * algorithm with a lower bound calculation based on matrix reduction. It finds the
 * minimum cost tour visiting each city exactly once and returning to the start.
 *
 * Key concepts: Branch and Bound, TSP, matrix reduction, lower bounds, priority queue.
 */

#include <iostream>    // For input/output operations (cin, cout)
#include <vector>      // For dynamic arrays (vector)
#include <queue>       // For priority queue
#include <limits>      // For numeric limits (INF)
#include <algorithm>   // For algorithms like min, max

using namespace std;  // Use the standard namespace

// Define infinity as the maximum int value
const int INF = numeric_limits<int>::max();
int N;  // Number of cities

// Structure to represent a node in the state space tree for Branch and Bound
struct Node {
    vector<vector<int>> mat;  // Reduced cost matrix for this node
    vector<int> path;         // Path taken so far (sequence of cities)
    int cost;                 // Cost of the path so far
    int lb;                   // Lower bound cost for this node
    int level;                // Number of cities visited (level in the tree)
    int curr_city;            // The current city this node is at
};

// Custom comparator for the min-priority queue (min-heap based on lower bound)
struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        return a.lb > b.lb;  // Smaller lower bound has higher priority
    }
};

// Function to perform row and column reduction on the cost matrix
// Modifies the matrix in-place and returns the total reduction cost (lower bound improvement)
int reduce_matrix(vector<vector<int>>& mat) {
    int reduction = 0;  // Total cost of reductions

    // Row reduction: Subtract the minimum value in each row from all elements in that row
    for (int i = 0; i < N; ++i) {
        int min_val = INF;  // Find minimum in row i
        for (int j = 0; j < N; ++j) {
            if (mat[i][j] < min_val)
                min_val = mat[i][j];
        }

        // If minimum is not 0 or INF, reduce the row
        if (min_val != 0 && min_val != INF) {
            for (int j = 0; j < N; ++j) {
                if (mat[i][j] != INF)
                    mat[i][j] -= min_val;  // Subtract min from each element
            }
            reduction += min_val;  // Add to total reduction
        }
    }

    // Column reduction: Subtract the minimum value in each column from all elements in that column
    for (int j = 0; j < N; ++j) {
        int min_val = INF;  // Find minimum in column j
        for (int i = 0; i < N; ++i) {
            if (mat[i][j] < min_val)
                min_val = mat[i][j];
        }

        // If minimum is not 0 or INF, reduce the column
        if (min_val != 0 && min_val != INF) {
            for (int i = 0; i < N; ++i) {
                if (mat[i][j] != INF)
                    mat[i][j] -= min_val;  // Subtract min from each element
            }
            reduction += min_val;  // Add to total reduction
        }
    }

    return reduction;  // Return the total reduction cost
}

// Helper function to check if a city is already visited in the current path
bool isVisited(const vector<int>& path, int city) {
    return find(path.begin(), path.end(), city) != path.end();  // Linear search in path vector
}

// Main function: Entry point of the program
int main() {
    cout << "Enter the number of cities (N): ";
    cin >> N;

    // Read the cost matrix from input
    vector<vector<int>> start_mat(N, vector<int>(N));
    cout << "Enter the cost matrix (use -1 or a large number for INF):\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> start_mat[i][j];
            if (start_mat[i][j] == -1)  // Convert -1 to INF
                start_mat[i][j] = INF;
        }
    }

    // Keep a copy of the original matrix for true travel costs (not reduced)
    vector<vector<int>> original = start_mat;

    // Priority queue to store live nodes (best-first search)
    priority_queue<Node, vector<Node>, CompareNode> pq;

    // Create the Root Node (starting point)
    Node root;
    root.mat = start_mat;        // Start with the original matrix
    root.path.push_back(0);      // Begin path at city 0
    root.cost = 0;               // No cost yet
    root.level = 0;              // Level 0: no cities visited
    root.curr_city = 0;          // Current city is 0
    root.lb = reduce_matrix(root.mat);  // Compute initial lower bound

    pq.push(root);  // Add root to the priority queue

    // Variables to track the best solution found
    int bestCost = INF;      // Best cost found so far
    vector<int> bestPath;    // Best path found

    // Main Branch and Bound Loop (LC: Least Cost)
    while (!pq.empty()) {
        Node node = pq.top();  // Get the node with smallest lower bound
        pq.pop();

        // Prune the node if its lower bound is worse than the best cost found
        if (node.lb >= bestCost)
            continue;

        // Goal check: If all cities are visited (level == N-1)
        if (node.level == N - 1) {
            // Check if there's a path back to city 0
            if (original[node.curr_city][0] != INF) {
                int final_cost = node.cost + original[node.curr_city][0];  // Add return cost
                if (final_cost < bestCost) {
                    bestCost = final_cost;  // Update best cost
                    bestPath = node.path;   // Update best path
                    bestPath.push_back(0);  // Add return to start
                }
            }
            continue;  // No further branching from leaf nodes
        }

        // Branching: Generate child nodes for each unvisited city
        for (int v = 0; v < N; ++v) {
            // If city v is not visited and there's an edge from current city
            if (!isVisited(node.path, v) && node.mat[node.curr_city][v] != INF) {
                Node child;  // Create a new child node
                child.mat = node.mat;       // Copy the reduced matrix
                child.path = node.path;     // Copy the path
                child.path.push_back(v);    // Add v to the path
                child.level = node.level + 1;  // Increase level
                child.curr_city = v;        // Set current city to v

                int u = node.curr_city;     // Parent city
                // Use original matrix for actual travel cost (not reduced)
                child.cost = node.cost + original[u][v];

                // Modify matrix: Set row u, column v, and v->0 to INF (no revisit)
                for (int j = 0; j < N; ++j)
                    child.mat[u][j] = INF;  // Row u: no outgoing from u
                for (int i = 0; i < N; ++i)
                    child.mat[i][v] = INF;  // Column v: no incoming to v
                child.mat[v][0] = INF;      // Prevent v->0 until end

                // Reduce the new matrix and compute lower bound
                int reduction = reduce_matrix(child.mat);
                child.lb = child.cost + reduction;

                // Push to queue if lower bound is promising
                if (child.lb < bestCost)
                    pq.push(child);
            }
        }
    }

    // Final Output: Display the best solution found
    if (bestCost == INF) {
        cout << "No feasible tour found." << endl;
    } else {
        cout << "Minimum Cost: " << bestCost << endl;
        cout << "Optimal Path: ";
        // Print the path with arrows
        for (size_t i = 0; i < bestPath.size(); ++i) {
            cout << bestPath[i];
            if (i < bestPath.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }

    return 0;  // Successful execution
}
