#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

// Structure for storing edges
struct Edge {
    int u, v, weight;
};

// Disjoint Set Union for Kruskal's Algorithm
class DSU {
    vector<int> parent, rank;

public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a != b) {
            if (rank[a] < rank[b])
                swap(a, b);

            parent[b] = a;

            if (rank[a] == rank[b])
                rank[a]++;
        }
    }
};

// Graph class
class Graph {
    int V;
    vector<vector<pair<int, int>>> adj;
    vector<Edge> edges;

    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (auto neighbour : adj[v]) {
            int next = neighbour.first;

            if (!visited[next])
                DFSUtil(next, visited);
        }
    }

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add an undirected weighted edge
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});

        edges.push_back({u, v, w});
    }

    // 1. Breadth First Search
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Traversal: ";

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            cout << v << " ";

            for (auto neighbour : adj[v]) {
                int next = neighbour.first;

                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        cout << endl;
    }

    // 2. Depth First Search
    void DFS(int start) {
        vector<bool> visited(V, false);

        cout << "DFS Traversal: ";
        DFSUtil(start, visited);

        cout << endl;
    }

    // 3. Kruskal's Algorithm
    void Kruskal() {
        vector<Edge> sortedEdges = edges;

        sort(sortedEdges.begin(), sortedEdges.end(),
             [](Edge a, Edge b) {
                 return a.weight < b.weight;
             });

        DSU dsu(V);
        int totalWeight = 0;
        int count = 0;

        cout << "Kruskal MST Edges:\n";

        for (auto e : sortedEdges) {
            if (dsu.find(e.u) != dsu.find(e.v)) {
                cout << e.u << " - " << e.v
                     << " : " << e.weight << endl;

                dsu.unite(e.u, e.v);
                totalWeight += e.weight;
                count++;
            }

            if (count == V - 1)
                break;
        }

        if (count != V - 1)
            cout << "Graph is disconnected. MST does not exist.\n";
        else
            cout << "Total MST Weight: " << totalWeight << endl;
    }

    // 4. Prim's Algorithm
    void Prim() {
        vector<bool> visited(V, false);
        priority_queue<pair<int, int>,
                       vector<pair<int, int>>,
                       greater<pair<int, int>>> pq;

        int totalWeight = 0;
        int count = 0;

        cout << "Prim MST Edges:\n";

        // Handles disconnected graphs as a minimum spanning forest
        for (int start = 0; start < V; start++) {
            if (visited[start])
                continue;

            pq.push({0, start});

            while (!pq.empty()) {
                int weight = pq.top().first;
                int u = pq.top().second;
                pq.pop();

                if (visited[u])
                    continue;

                visited[u] = true;
                totalWeight += weight;
                count++;

                if (weight != 0)
                    cout << "Selected vertex: " << u
                         << " with edge weight: " << weight << endl;

                for (auto neighbour : adj[u]) {
                    int v = neighbour.first;
                    int w = neighbour.second;

                    if (!visited[v])
                        pq.push({w, v});
                }
            }
        }

        cout << "Total MST Weight / Forest Weight: "
             << totalWeight << endl;

        if (count != V)
            cout << "Some vertices are not included.\n";
    }

    // 5. Dijkstra's Shortest Path Algorithm
    void Dijkstra(int source) {
        vector<int> dist(V, INT_MAX);

        priority_queue<pair<int, int>,
                       vector<pair<int, int>>,
                       greater<pair<int, int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d != dist[u])
                continue;

            for (auto neighbour : adj[u]) {
                int v = neighbour.first;
                int weight = neighbour.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Shortest distances from vertex "
             << source << ":\n";

        for (int i = 0; i < V; i++) {
            cout << "Vertex " << i << " : ";

            if (dist[i] == INT_MAX)
                cout << "INF";
            else
                cout << dist[i];

            cout << endl;
        }
    }
};

// Main Function
int main() {
    int V, E;

    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter edges (u v weight):\n";

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        g.addEdge(u, v, w);
    }

    int choice, start;

    do {
        cout << "\n----- GRAPH ALGORITHMS MENU -----\n";
        cout << "1. Breadth First Search (BFS)\n";
        cout << "2. Depth First Search (DFS)\n";
        cout << "3. Kruskal's Algorithm\n";
        cout << "4. Prim's Algorithm\n";
        cout << "5. Dijkstra's Algorithm\n";
        cout << "6. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter starting vertex: ";
                cin >> start;
                if (start >= 0 && start < V)
                    g.BFS(start);
                else
                    cout << "Invalid vertex!\n";
                break;

            case 2:
                cout << "Enter starting vertex: ";
                cin >> start;
                if (start >= 0 && start < V)
                    g.DFS(start);
                else
                    cout << "Invalid vertex!\n";
                break;

            case 3:
                g.Kruskal();
                break;

            case 4:
                g.Prim();
                break;

            case 5:
                cout << "Enter source vertex: ";
                cin >> start;
                if (start >= 0 && start < V)
                    g.Dijkstra(start);
                else
                    cout << "Invalid vertex!\n";
                break;

            case 6:
                cout << "Program exited.\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}
