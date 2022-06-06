#include "../include/Graph.h"

Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {
}

void Graph::addEdge(int src, int dest, int capacity, int hours) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, capacity,hours});
    if (!hasDir) nodes[dest].adj.push_back({src, capacity,hours});
}

int Graph::getNumNodes() {
    return n;
}

vector<Graph::Node> &Graph::getNodes() {
    return nodes;
}

int Graph::addNode(int node) {
    int index = (int)nodes.size();
    for (int i=1; i <= n; i++)
        if(i==node) return i;
    Node nd = Node();
    nodes.push_back(nd);
    n++;
    return index;
}


