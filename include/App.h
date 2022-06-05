#ifndef PROJETO2_DA_APP_H
#define PROJETO2_DA_APP_H

#include <string>
#include <map>
#include <algorithm>
#include <stack>
#include <sstream>
#include "Graph.h"
#include "MaxHeap.h"
#include <vector>
#include <queue>


/**
 * Main class with all the algorithms that solve the proposed scenarios
 */
class App {

private:
    std::string dataFolder = "../data/";
    std::string dataName = "in";
    std::string currGraph;
    std::string fileExtension = ".txt";
    std::vector<int> stops; // for 1.1-1.2
    Graph graph = Graph();
    Graph auxilarGraph = Graph(); // for 2.4/2.5
public:
    vector<int> parent; // for 2.1-2.5
    vector<pair<vector<int>, int>> paretoPaths; // for 1.2
    pair<map<pair<int,int>, int>, int> pathsMap; // for 2.1-2.5
    vector<vector<pair<int,int>>> capacitiesGraph; // for 2.1-2.5
    vector<int> lastPathInfo{0,0,0}; // for 2.1-2.5

public:
    /**
     * App class default constructor
     */
    App();
    /**
     * App class default destructor
     */
    ~App();
    /**
     * Calls all the methods to read the files and load the program
     * @param file index number of the file in filesname vector
     */
    void readData();
    std::string getGraphStr();
    void setGraphStr(std::string graphName);
    Graph &getGraph();
    Graph &getAuxGraph();
    int maximumCapacityPath(int source, int destination); // SCENARIO 1.1
    pair<int, vector<Graph::Node>> earliestStart(Graph &graph);
    pair<int, vector<int>> maxWaitTime(Graph &graph);

    std::vector<int> getPathScenario_1(int source, int destiny);
    void printPathScenario1_2();
    void printPathScenario_2();

    void filterPaths1_2();
    void edmondsKarp1(int origin, int dest);
    Graph edmondsKarp2(int origin, int dest, int size, bool increase, bool findMax);

    bool bfs(vector<vector<pair<int, int>>> flowGraph, int origin, int destination, int parent[], int nodeSize);
};

#endif //PROJETO2_DA_APP_H