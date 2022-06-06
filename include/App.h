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
    std::string dataFolder = "../input/";
    std::string dataName = "in";
    std::string bType = "_b";
    std::string graphType = "";
    std::string currGraph;
    std::string fileExtension = ".txt";
    /** vector of node indexes used in scenario 1*/
    std::vector<int> stops;
    Graph graph = Graph();
    /** Auxiliary graph used in 2.4 and 2.5, representing the path that the group followed */
    Graph auxilarGraph = Graph();
public:
    /** Vector of parent nodes using in Scenario 2*/
    vector<int> parent;
    /** Vector that stores a path and its maximum capacity using in Scenario 1.2 for pareto-optimum paths*/
    vector<pair<vector<int>, int>> paretoPaths;
    /** A pair of map and integer that stores information after algorithm executed used in Scenario 2*/
    pair<map<pair<int,int>, int>, int> pathsMap;
    /** Vector of capacities used in Edmund's Karp algorithms, Scenario 2*/
    vector<vector<pair<int,int>>> capacitiesGraph;
    /** Vector that stores the source, destination and group size of the last user input*/
    vector<int> lastPathInfo{0,0,0};

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
    /**
     * Get the string number of the current graph in use
     * @return string with the number of the input graph
     */
    std::string getGraphStr();
    /**
     * Sets the current graph name to the given parameter
     * @param graphName string with the number of the graph to be used
     */
    void setGraphStr(std::string graphName);
    /**
     * Gets the graph type string of the graph in use
     * @return empty string in the type is the default ou "_b" if B type graph is in use
     */
    std::string getGraphType();
    /**
     * After called switch de state of the graph type, if default type is the current switches to B type and vice-versa
     */
    void toggleGraphType();
    /**
     * Get the current graph that is used in the program
     * @return reference to the current graph
     */
    Graph &getGraph();
    /**
     * Get the graph of the path followed by the group in the last Scenario 2 execution
     * @return reference to the auxilarGraph
     */
    Graph &getAuxGraph();
    /**
     * Scenario 1.1
     * Calculates maximum group size of a trip given the source and destination node, without splitting the group
     * @param source index source node in the nodes vector of the graph
     * @param destination index destination node in the nodes vector of the graph
     * @return maximum group size for the path
     */
    int maximumCapacityPath(int source, int destination);
    /**
     * Scenario 2.4
     * Calculates the earliest start of each node in the given graph, representing a path of a group
     * @param graph graph object passed by reference
     * @return a pair with first being the minimum duration of the trip represented by the graph and the second is a vector of the graph's nodes
     */
    pair<int, vector<Graph::Node>> earliestStart(Graph &graph);
    /**
     * Scenario 2.5
     * Calculates the maximum wait time in each node of the given graph, representing a path of a group
     * @param graph graph object passed by reference
     * @return a pair with first element being the maximum wait time, and the second element is a vector of the nodes where it happens
     */
    pair<int, vector<int>> maxWaitTime(Graph &graph);
    /**
     * Gets the path for scenario to options given a source and destination node
     * @param source source node in the graph
     * @param destiny destination node in the graph
     * @return vector of the nodes representing the path followed
     */
    std::vector<int> getPathScenario_1(int source, int destiny);
    /**
     * Prints the path of Scenario 1.2 in the console
     */
    void printPathScenario1_2();
    /**
     * Prints the path for Scenarios 2.*
     */
    void printPathScenario_2();
    /**
     * Filter the pareto-optimum paths of the Scenario 1.2
     */
    void filterPaths1_2();
    /**
     * Edmonds Karp algorithm used in the first Scenario 1.2
     * @param origin source node in the graph
     * @param dest destination node in the graph
     */
    void edmondsKarp1(int origin, int dest);
    /**
     * Edmonds Karp algorithm used in the Second Scenarios
     * @param origin source node in the graph
     * @param dest destination node in the graph
     * @param size group size fot the trip
     * @param increase true if you want to increase the group capacity of a path that was found before or false otherwise
     * @param findMax true if you want to find the path with the maximum group size capacity or false otherwise
     * @return a graph of the path that the group followed, which is used in Scenario 2.4 and 2.5
     */
    Graph edmondsKarp2(int origin, int dest, int size, bool increase, bool findMax);
    /**
     * Bfs algorithm that runs in a graph represented by a vector of nodes which are other vectors, and contain pairs where the
     * @param flowGraph vector of nodes which are other vectors, and contain pairs where the first element is the flow of people in that edges and the second is the maximum capacity of the edge
     * @param origin source node in the graph
     * @param destination destination node in the graph
     * @param parent return by reference and array of nodes indexes that are parents of the nodes in the flowGraph vector
     * @param nodeSize number of nodes in the graph
     * @return true if the algorithm can find a path from the source to the destination or false otherwise
     */
    bool bfs(vector<vector<pair<int, int>>> flowGraph, int origin, int destination, int parent[], int nodeSize);
};

#endif //PROJETO2_DA_APP_H