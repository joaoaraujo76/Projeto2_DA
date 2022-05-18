#include "../include/Graph.h"

Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {
}

void Graph::addEdge(int src, int dest, int capacidade, int horas) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, capacidade,horas});
    if (!hasDir) nodes[dest].adj.push_back({src, capacidade,horas});
}

void Graph::dfs(int v) {
    nodes[v].visited=true;
    for(auto edge : nodes[v].adj){
        if(!nodes[edge.dest].visited){
            dfs(edge.dest);
        }
    }
}

void Graph::bfs(int v) {
    queue<int> q;
    q.push(v);
    nodes[v].visited = true;
    while(q.size() > 0){
        int u = q.front();
        q.pop();
        for(auto e: nodes[u] .adj){
            int w = e.dest;
            if(!nodes[w].visited){
                q.push(w);
                nodes[w].visited = true;
            }
        }
    }
}


vector<Graph::Node> Graph::getnodes(){
    return nodes;
}


