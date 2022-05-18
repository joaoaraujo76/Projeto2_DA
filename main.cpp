#include <iostream>
#include "include/graph.h"
#include <fstream>

int main() {
    ifstream myfile;
    int nodes, edges, origem, destino, capacidade, duracao;
    myfile.open("../data/in01.txt");
    if(!myfile.is_open()){
        cout << "Tudo fodido!";
        exit(1);
    }
    myfile >> nodes;
    myfile >> edges;
    Graph g(nodes,true);
    while (myfile >> origem >> destino >> capacidade >> duracao){
        g.addEdge(origem,destino,capacidade,duracao);
    }
    for(int j = 0 ; j < nodes; j++){
        cout << endl << "Destinos a partir de [" << j <<"] : ";
        for(int i = 0; i < g.getnodes()[j].adj.size(); i++){
            cout << g.getnodes().at(j).adj.size();
        }
    }
    return 0;
}
