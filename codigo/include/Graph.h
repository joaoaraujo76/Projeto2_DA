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
        /** Destination node */
        int dest;
        /** Capacity of the edge */
        int cap;
        /** Time spent in the edge */
        int duration;
    };

    struct Node {
        /** The list of outgoing edges (to adjacent nodes) */
        list<Edge> adj;
        bool visited;
        int maxCap;
        int parent;
        int ES;
        int eDegree;
    };
    /** false: undirect; true: directed */
    bool hasDir;
    /** Number of node in the graph */
    int n;
    /** The list of nodes being represented */
    vector<Node> nodes = {{}};

    /**
     * Constructor of a graph with the number of nodes and direction of the edges
     * @param num number of the nodes
     * @param dir true if graph is directed or false otherwise
     */
    explicit Graph(int num = 0, bool dir = true);
    /**
     * Adds an edges to the graph given a source, destination node, maximum capacity for the edge and duration of the trip between nodes
     * @param src source node
     * @param dest destination node
     * @param capacidade maximum capacity for the edge
     * @param horas duration of the trip between nodes
     */
    void addEdge(int src, int dest, int capacidade, int horas);
    /**
     * Adds a node to the graph, matching the node parameter and if already exists just return his index in the nodes vector
     * @param node node number to be added
     * @return node index of the node added or the node index if it already exists
     */
    int addNode(int node);
    /**
     * Get the number of the nodes in the graph
     * @return n value of the graph
     */
    int getNumNodes();
    /**
     * Gets the nodes vector of the graph
     * @return a reference to the nodes vector of the graph
     */
    vector<Node> &getNodes();
};

#endif
