#ifndef PROJETO2_DA_APP_H
#define PROJETO2_DA_APP_H

#include <string>
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
    Graph graph;
    std::vector<int> stops;

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
     * Checks if the argument file exists and clears it, otherwise creates one file matching de FILE_NUM argument correspondent in the filesname
     * @param file fstream pointer file to clear
     * @param FILE_NUM index file number in the filesname vector
     * @return true if file exists or false otherwise
     */
    bool clearFile(std::fstream *file, int FILE_NUM);
    /**
     * Checks if the argument file exists and opens it in ios::app mode, otherwise creates one file matching de FILE_NUM argument correspondent in the filesname
     * @param file fstream pointer file to open
     * @param FILE_NUM index file number in the filesname vector
     * @return true if file exists or false otherwise
     */
    bool createFile(std::fstream *file, int FILE_NUM);
    /**
     * Reads the settings from the settings text file and loads them in the program
     */
    void readSettings();
    /**
     * Checks if the file passed in the argument is empty
     * @param file fstream pointer file to check if it's empty
     * @param FILE_NUM index file number in filesname vector
     * @return true is file is empty or false otherwise
     */
    bool emptyFile(std::fstream *file, int FILE_NUM);
    /**
     * Writes the program actual settings in the settings file
     */
    void writeSettings();
    std::string getGraph();
    void setGraph(std::string graphName);
    int maximumCapacityPath(int source, int destination); // SCENARIO 1.1
    pair<int, vector<Graph::Node>> earliestStart(Graph &graph);
    void latestFinish(Graph &graph);

    std::vector<int> getPath(int source, int destiny);
};

#endif //PROJETO2_DA_APP_H