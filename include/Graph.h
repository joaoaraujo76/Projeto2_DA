#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <climits>

#define INF (INT_MAX/2)
using namespace std;

class Graph {
public:
    struct Edge {
        int dest;   // Destination node
        int cap;
        int duration;
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        bool visited;   // As the node been visited on a search?
        int distance;
        int maxCap;
        int parent;
        int LF;
        int ES;
        int eDegree;
        int sDegree;
    };
    bool hasDir;        // false: undirect; true: directed
    int n;

    vector<Node> nodes = {{}}; // The list of nodes being represented
    // Constructor: nr nodes and direction (default: undirected)
    explicit Graph(int num = 0, bool dir = false);

    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, int capacidade, int horas);

    // Depth-First Search: example implementation
    void dfs(int v);

    // Breadth-First Search: example implementation
    void bfs(int v);

    int getNumNodes();

    void clear(int nodesNum);

    Graph transpose();
};

#endif
