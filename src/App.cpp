#include <fstream>
#include <queue>
#include "../include/App.h"

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
    graph.clear(nodes);
    while (dataFile >> source >> destiny >> capacity >> duration){
        //cout << "origem " << origem << " destino " << destino << " capacidade " << capacidade <<  " duracao " << duracao << "\n";
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

string App::getGraph() {
    return currGraph;
}

void App::setGraph(string graphName) {
    currGraph = graphName;
    readData();
}


int App::maximumCapacityPath(int source, int destination) {
    if(source > graph.getNumNodes() || destination > graph.getNumNodes() || source < 1 || destination < 1)
        return -1;
    MaxHeap<int,int>q(graph.getNumNodes(),-1);
    for(int i = 1; i <= graph.getNumNodes(); i++){
        graph.nodes[i].capacity = INF * -1;
        graph.nodes[i].parent = 0;
        q.insert(i,graph.nodes[i].capacity);
    }
    graph.nodes[source].capacity = INF;

    q.increaseKey(source, INF);

    int v;
    while(q.getSize() > 0){
        v = q.removeMax();
        /*cout << "after remove size is " << q.getSize() << "\n";
        cout << v << endl;*/
        for(auto w : graph.nodes[v].adj){
            /*cout << "|" << w.dest << "|" << endl;
            cout << "capacidade do no " << v  << " = " << g.nodes[v].capacity << "\n";
            cout << "capacidade da aresta até " << w.dest  << " = " << w.cap << "\n";*/
            if(min(graph.nodes[v].capacity,w.cap) > graph.nodes[w.dest].capacity){
                //cout << "entrou no if\n";
                graph.nodes[w.dest].capacity = min(graph.nodes[v].capacity,w.cap);
                graph.nodes[w.dest].parent = v;
                q.increaseKey(w.dest, graph.nodes[w.dest].capacity);
                //cout << "node " << w.dest << " capacity = " << g.nodes[w.dest].capacity << "\n";
            }
        }
    }
    return graph.nodes[destination].capacity;
}

vector<int> App::getPath(int source, int destination) {
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

int App::earliestStart(Graph &graph){
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
            if (graph.nodes[w.dest].ES < graph.nodes[v].ES + w.horas){
                graph.nodes[w.dest].ES = graph.nodes[v].ES + w.horas;
                graph.nodes[w.dest].parent = v;
            }
            graph.nodes[w.dest].eDegree = graph.nodes[w.dest].eDegree - 1;
            if (graph.nodes[w.dest].eDegree == 0)
                S.push(w.dest);
        }
    }
    return minDuration;
}

void App::latestFinish(Graph &graph) {
    int minDuration = earliestStart(graph);
    for(int i = 1; i <= graph.getNumNodes(); i++){
        graph.nodes[i].LF = minDuration;
        graph.nodes[i].sDegree = 0;
    }

    for(int i = 1; i <= graph.getNumNodes(); i++){
        for(Graph::Edge w : graph.nodes[i].adj){
            graph.nodes[w.dest].sDegree += 1;
        }
    }

    Graph transposed = graph;
    vector<Graph::Node> nodes;
    int aux;
    for(int i = 1; i <= transposed.getNumNodes(); i++){
        for(auto w : transposed.nodes[i].adj){

        }
    }
}