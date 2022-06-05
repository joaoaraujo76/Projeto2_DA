#include "../include/App.h"

#include <fstream>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

App::App() = default;
App::~App() = default;

void App::readData() {
    ifstream dataFile;
    int nodes, edges, source, destiny, capacity, duration;
    dataFile.open(dataFolder + dataName + currGraph + graphType + fileExtension);
    if(!dataFile.is_open()){
        cerr << "Couldn't open " << dataName + currGraph + fileExtension << " file";
        exit(1);
    }
    dataFile >> nodes;
    dataFile >> edges;
    graph = Graph();
    parent.clear();
    parent.push_back(0);
    for (int i=1; i<=nodes; i++){
        graph.addNode(i);
        parent.push_back(0);
    }

    while (dataFile >> source >> destiny >> capacity >> duration){
        graph.addEdge(source,destiny,capacity,duration);

    }
    dataFile.close();
}

string App::getGraphStr() {
    return currGraph;
}

void App::setGraphStr(string graphName) {
    currGraph = graphName;
    readData();
}


int App::maximumCapacityPath(int source, int destination) {
    if(source > graph.getNumNodes() || destination > graph.getNumNodes() || source < 1 || destination < 1)
        return -1;
    MaxHeap<int,int>q(graph.getNumNodes(),-1);
    for(int i = 1; i <= graph.getNumNodes(); i++){
        graph.nodes[i].maxCap = INF * -1;
        graph.nodes[i].parent = 0;
        q.insert(i,graph.nodes[i].maxCap);
    }
    graph.nodes[source].maxCap = INF;

    q.increaseKey(source, INF);

    int v;
    while(q.getSize() > 0){
        v = q.removeMax();
        for(auto w : graph.nodes[v].adj){
            if(min(graph.nodes[v].maxCap, w.cap) > graph.nodes[w.dest].maxCap){
                graph.nodes[w.dest].maxCap = min(graph.nodes[v].maxCap, w.cap);
                graph.nodes[w.dest].parent = v;
                q.increaseKey(w.dest, graph.nodes[w.dest].maxCap);
            }
        }
    }
    if(graph.nodes[destination].maxCap < 0)
        return -2;
    return graph.nodes[destination].maxCap;
}

vector<int> App::getPathScenario_1(int source, int destination) {
    stops.clear();
    int final = destination;
    stops.push_back(destination);
    do{
        stops.push_back(graph.nodes[final].parent);
        final = graph.nodes[final].parent;
    }while(final != source);
    reverse(stops.begin(), stops.end());
    return stops;
}

pair<int, vector<Graph::Node>> App::earliestStart(Graph &graph){
    int minDuration = -1;
    int vf = 0;
    queue<int> S;
    for(int i = 1; i <= graph.getNumNodes(); i++){
        graph.nodes[i].ES = 0;
        graph.nodes[i].parent = 0;
        graph.nodes[i].eDegree = 0;
    }
    for(int i = 1; i <= graph.getNumNodes(); i++){
        for (Graph::Edge e : graph.nodes[i].adj){
            graph.nodes[e.dest].eDegree += 1;
        }
    }
    for(int i = 1; i <= graph.getNumNodes(); i++){
        if(graph.nodes[i].eDegree == 0) S.push(i);
    }

    while (!S.empty()){
        int v = S.front();
        S.pop();
        if (minDuration < graph.nodes[v].ES){
            minDuration = graph.nodes[v].ES;
            vf = v;
        }
        for (auto w : graph.nodes[v].adj){
            if (graph.nodes[w.dest].ES < graph.nodes[v].ES + w.duration){
                graph.nodes[w.dest].ES = graph.nodes[v].ES + w.duration;
                graph.nodes[w.dest].parent = v;
            }
            graph.nodes[w.dest].eDegree = graph.nodes[w.dest].eDegree - 1;
            if (graph.nodes[w.dest].eDegree == 0)
                S.push(w.dest);
        }
    }
    return {minDuration, graph.nodes};
}

pair<int, vector<int>> App::maxWaitTime(Graph &graph) {
    queue<int> S;
    pair<int, vector<Graph::Node>> eS = earliestStart(graph);
    vector<int> stations;
    vector<int> wait(graph.getNumNodes()+1,0);

    for(int i = 1; i <= graph.getNumNodes(); i++){
        for (auto e : eS.second[i].adj){
            int newMax = eS.second[e.dest].ES - eS.second[i].ES - e.duration;
            if(wait[e.dest] < newMax)
                wait[e.dest] = newMax;
        }
    }
    int maxWait = INT_MIN;
    for(int i : wait){
        if(maxWait < i) maxWait = i;
    }
    for(int i = 1; i <= wait.size(); i++) {
        if (wait[i] == maxWait) {
            stations.push_back(i);
        }
    }
    return {maxWait, stations};
}

Graph App::edmondsKarp2(int origin, int dest, int size, bool increase, bool findMax) {

    if(lastPathInfo[0] == origin && lastPathInfo[1] == dest) {
        increase = true;
    }

    vector<Graph::Node> nodes = graph.getNodes();
    lastPathInfo = {origin, dest, size}; // Update lastPathInfo with new data
    int u, v, nodeSize = nodes.size();
    int auxParent[nodeSize];
    if(!increase) {
        auxilarGraph = Graph(0, true);
        if(!capacitiesGraph.empty()) capacitiesGraph.clear();
        for (u = 0; u < nodeSize; u++) {
            if(u > 0) auxilarGraph.addNode(u);
            parent.at(u) = 0;
            vector<pair<int, int>> vec;
            for (v = 0; v < nodeSize; v++){
                bool breaker = false;
                for(auto edge : nodes[u].adj) {
                    if(edge.dest == v) {
                        if(breaker) {
                            vec.back().first +=edge.cap;
                            continue;
                        }
                        else {
                            vec.emplace_back(edge.cap, edge.duration);
                            breaker = true;
                        }
                    }
                }
                if(!breaker) vec.emplace_back(0,0);
            }
            capacitiesGraph.push_back(vec);
        }
        pathsMap.first.clear();
        pathsMap.second = 0;
    }
    else {
        for(int i = 0; i < parent.size(); i++) {
            auxParent[i] = parent.at(i);
        }
    }
    while (bfs(capacitiesGraph, origin, dest, auxParent, nodeSize)) {
        int path_flow = INF;

        for (v = dest; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            path_flow = min(path_flow, capacitiesGraph[u][v].first);
        }
        for (v = dest; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            capacitiesGraph[u][v].first -= path_flow;
            capacitiesGraph[v][u].first += path_flow;
            if(pathsMap.first.find(make_pair(u, v)) == pathsMap.first.end()) {
                pathsMap.first.insert(pair<pair<int,int>, int> (make_pair(u, v), path_flow));
                auxilarGraph.addEdge(u, v, 1, capacitiesGraph[u][v].second);
            }
            else {
                pathsMap.first.find(make_pair(u, v))->second += path_flow;
            }
        }
        pathsMap.second += path_flow;
        if(findMax);
        else if(pathsMap.second >= size) break;
    }
    for(int i = 1; i < nodeSize; i++) {
        parent.at(i) = auxParent[i];
    }
    if(findMax) {
        lastPathInfo = {origin, dest, pathsMap.second}; // Update lastPathInfo with new data
    }

    return auxilarGraph;
}


void App::edmondsKarp1(int origin, int dest) {
    int u, v, V;
    vector<Graph::Node> nodes = graph.getNodes();
    V = nodes.size();

    vector<vector<pair<int, int>>> rGraph;

    for (u = 0; u < V; u++) {
        if(u > 0) auxilarGraph.addNode(u);
        parent.at(u) = 0;
        vector<pair<int, int>> vec;
        for (v = 0; v < V; v++){
            bool breaker = false;
            for(auto edge : nodes[u].adj) {
                if(edge.dest == v) {
                    if(breaker) {
                        vec.back().first +=edge.cap;
                        vec.back().second +=edge.cap;
                        continue;
                    }
                    else {
                        vec.emplace_back(edge.cap, edge.cap);
                        breaker = true;
                    }
                }
            }
            if(!breaker) vec.emplace_back(0,0);
        }
        rGraph.push_back(vec);
    }

    int auxParent[V];
    while (bfs(rGraph, origin, dest, auxParent, V)) {

        int path_flow = INF;
        for (v = dest; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            path_flow = min(path_flow, rGraph[u][v].first);
        }

        pair<vector<int>, int> auxPath;
        auxPath.second = INF/2;
        auxPath.first.push_back(dest);
        for (v = dest; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            rGraph[u][v].first -= path_flow;
            rGraph[v][u].first += path_flow;
            auxPath.second = min(auxPath.second, rGraph[u][v].second);
            auxPath.first.push_back(u);
        }
        reverse(auxPath.first.begin(), auxPath.first.end());
        paretoPaths.emplace_back(auxPath);
    }
}

bool App::bfs(vector<vector<pair<int, int>>> flowGraph, int origin, int destination, int *parent, int nodeSize) {
    bool visited[nodeSize];
    memset(visited, 0, sizeof(visited));

    queue<int> q;
    q.push(origin);
    visited[origin] = true;
    parent[origin] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < nodeSize; v++) {
            if (visited[v] == false && flowGraph[u][v].first > 0) {
                if (v == destination) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

Graph &App::getGraph() {
    return graph;
}

Graph &App::getAuxGraph() {
    return auxilarGraph;
}

void App::printPathScenario_2() {
    for(auto m : pathsMap.first) {
        cout << m.first.first << " -> " << m.first.second << endl;
    }
}

void App::filterPaths1_2() {
    sort(paretoPaths.begin(), paretoPaths.end(), [](pair<vector<int>, int>&element1, pair<vector<int>, int>&element2){
        if(element1.second != element2.second) {
            return element1.second < element2.second;
        }
        int weightEle1 = element1.first.size(), weightEle2 = element2.first.size();
        return weightEle1 > weightEle2;
    });
    reverse(paretoPaths.begin(), paretoPaths.end());
    int max_c = paretoPaths[0].second + 1, min_path = paretoPaths[0].first.size() + 1;
    int i;
    for(i = 0; paretoPaths.at(i) != paretoPaths.back(); ) {
        if(paretoPaths.at(i).second == max_c) {
            if(paretoPaths.at(i).first.size() >= min_path) {
                paretoPaths.erase(paretoPaths.begin() + i);
                continue;
            }
            else
                min_path = paretoPaths.at(i).first.size();
        } else {
            max_c = paretoPaths.at(i).second;
            if(paretoPaths.at(i).first.size() >= min_path) {
                paretoPaths.erase(paretoPaths.begin() + i);
                continue;
            }
            else
                min_path = paretoPaths.at(i).first.size();
        }
        i++;
    }

    if(paretoPaths.back().first.size() >= min_path)
        paretoPaths.erase(paretoPaths.begin() + i);

    reverse(paretoPaths.begin(), paretoPaths.end());
}

void App::printPathScenario1_2() {
    if(paretoPaths.empty()) {
        cout << "No path available "<<endl;
    }
    for(const auto& path : paretoPaths) {
        cout << "Maximum capacity "<< path.second << ", stops " << path.first.size() << endl;
        int stopsNum = 1;
        for(auto stop : path.first) {
            cout << stop;
            if(stopsNum == path.first.size()) break;
            cout << " -> ";
            stopsNum++;
        }
        cout << endl << endl;
    }
}

std::string App::getGraphType() {
    return graphType;
}

void App::toggleGraphType() {
    graphType = graphType.empty()?bType:"";
}
