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
    dataFile.open(dataFolder + dataName + currGraph + fileExtension);
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
        //cout << "origem " << source << " destino " << destiny << " capacidade " << capacity <<  " duracao " << duration << "\n";
        graph.addEdge(source,destiny,capacity,duration);

    }
    dataFile.close();
}


/*void App::readSettings() {
    fstream settingsFile;
    createFile(&settingsFile, SETSFILE);
    if (emptyFile(&settingsFile, SETSFILE)) {
        settingsFile << "work time (hours) -10" << endl;
        settingsFile << "max express delivery duration (minutes) -4" << endl;
        settingsFile.close();
    }
    ifstream file(dataFolder + filesname[3]);
    string line;
    getline(file,line, '-');
    file >> workTime;
    setWorkingTime(workTime);
    getline(file,line, '-');
    file >> maxExpressDuration;
    setMaxExpressDuration(maxExpressDuration);
    file.close();
}*/

/*void App::writeSettings() {
    fstream settingsFile;
    clearFile(&settingsFile,SETSFILE);
    settingsFile << "work time (hours) -" << workTime / 3600 << endl;
    settingsFile << "max express delivery duration (minutes) -" << maxExpressDuration / 60 << endl;
    settingsFile.close();
}*/

/*bool App::createFile(std::fstream *file, int FILE_NUM) {
    bool created = false;
    file->open(dataFolder + filesname[FILE_NUM]);
    if(!file->is_open()){
        fstream newFile(dataFolder + filesname[FILE_NUM]);
        file = &newFile;
        newFile.open(dataFolder + filesname[FILE_NUM], ios::app);
        created = true;
        if (!newFile.is_open()) {
            cerr << "Unable to open file";
            exit(1);
        }
    }
    file->close();
    return created;
}*/

/*bool App::clearFile(std::fstream *file, int FILE_NUM) {
    bool exists = true;
    file->open(dataFolder + filesname[FILE_NUM], ofstream::out | ofstream::trunc);
    if(!file->is_open()){
        fstream newFile(dataFolder + filesname[FILE_NUM]);
        file = &newFile;
        newFile.open(dataFolder + filesname[FILE_NUM], ofstream::out | ofstream::trunc);
        exists = false;
        if (!newFile.is_open()) {
            cerr << "Unable to open file";
            exit(1);
        }
    }
    return exists;
}*/

/*bool App::emptyFile(fstream *file, const int FILE_NUM) {
    file->open(dataFolder + filesname[FILE_NUM]);
    file->seekg(0, ios::end);
    if (file->tellg() == 0) {
        return true;
    }
    return false;
}*/

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

Graph App::edmondsKarp(int origin, int destination, int size, bool increase, bool findMax) {

    if(lastPathInfo[0] == origin && lastPathInfo[1] == destination) {
        increase = true;
    }

    vector<Graph::Node> nodes = graph.getNodes();
    lastPathInfo = {origin, destination, size}; // Update lastPathInfo with new data
    int u, v, nodeSize = nodes.size();
    int auxParent[nodeSize];
    if(!increase) {
        auxGraph = Graph(0, true);
        if(!flowGraph.empty()) flowGraph.clear();
        for (u = 0; u < nodeSize; u++) {
            if(u > 0) auxGraph.addNode(u);
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
            flowGraph.push_back(vec);
        }
        pathsMap.first.clear();
        pathsMap.second = 0;
    }
    else {
        for(int i = 0; i < parent.size(); i++) {
            auxParent[i] = parent.at(i);
        }
    }
    while (bfs(flowGraph, origin, destination, auxParent, nodeSize)) {
        int path_flow = INT_MAX;

        for (v = destination; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            path_flow = min(path_flow, flowGraph[u][v].first);
        }
        for (v = destination; v != origin; v = auxParent[v]) {
            u = auxParent[v];
            flowGraph[u][v].first -= path_flow;
            flowGraph[v][u].first += path_flow;
            if(pathsMap.first.find(make_pair(u, v)) == pathsMap.first.end()) {
                pathsMap.first.insert(pair<pair<int,int>, int> (make_pair(u, v), path_flow));
                auxGraph.addEdge(u,v, 1, flowGraph[u][v].second);
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
        lastPathInfo = {origin, destination, pathsMap.second}; // Update lastPathInfo with new data
    }

    return auxGraph;
}

bool App::bfs(vector<vector<pair<int, int>>> flowGraph, int origin, int destination, int *parent, int nodeSize) {
    bool visited[nodeSize];
    memset(visited, 0, sizeof(visited));

    queue<int> q;
    q.push(origin);
    visited[origin] = true;
    parent[origin] = -1;

    // Standard bfs loop
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
    // We didn'destination reach the destination so we return false
    return false;
}

Graph &App::getGraph() {
    return graph;
}

Graph &App::getAuxGraph() {
    return auxGraph;
}

void App::getPathScenario_2() {
    for(auto m : pathsMap.first) {
        cout << m.first.first << " -> " << m.first.second << endl;
    }
}
