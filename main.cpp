#include <iostream>
#include "include/Graph.h"
#include <fstream>

#include "include/MaxHeap.h"

int main() {
    ifstream myfile;
    int node, edge, origem, destino, capacidade, duracao;
    myfile.open("data/in05.txt");
    if(!myfile.is_open()){
        cout << "Tudo fodido!";
        exit(1);
    }
    myfile >> node;
    myfile >> edge;
    Graph g(node, false);
    while (myfile >> origem >> destino >> capacidade >> duracao){
        //cout << "origem " << origem << " destino " << destino << " capacidade " << capacidade <<  " duracao " << duracao << "\n";
        g.addEdge(origem,destino,capacidade,duracao);
    }
    myfile.close();
    /*for(int j = 0 ; j < nodes; j++){
        cout << endl << "Destinos a partir de [" << j <<"] : ";
        for(int i = 0; i < g.getnodes()[j].adj.size(); i++){
            cout << g.getnodes().at(j).adj.size();
        }
    }*/

    int source = 1, destination = 3;
    MaxHeap<int,int>q(g.numNodes(),-1);
    for(int i = 1; i <= g.nodes.size(); i++){
        g.nodes[i].capacity = 0;
        g.nodes[i].parent = 0;
        q.insert(i,g.nodes[i].capacity);
    }
    g.nodes[source].capacity = INF;
    int v;
    while(q.getSize() > 0){
        v = q.removeMax();
        /*cout << "after remove size is " << q.getSize() << "\n";
        cout << v << endl;*/
        for(auto w : g.nodes[v].adj){
            /*cout << "|" << w.dest << "|" << endl;
            cout << "capacidade do no " << v  << " = " << g.nodes[v].capacity << "\n";
            cout << "capacidade da aresta até " << w.dest  << " = " << w.cap << "\n";*/
            if(min(g.nodes[v].capacity,w.cap) > g.nodes[w.dest].capacity){
                //cout << "entrou no if\n";
                g.nodes[w.dest].capacity = min(g.nodes[v].capacity,w.cap);
                g.nodes[w.dest].parent = v;
                q.decreaseKey(w.dest, g.nodes[w.dest].capacity);
                //cout << "node " << w.dest << " capacity = " << g.nodes[w.dest].capacity << "\n";
            }
        }
    }
    cout << "MAXIMA CAPACIDADE MINIMA -> " << g.nodes[destination].capacity << endl;
    cout << "CAMINHO:";
    vector<int> course;
    int final = destination;
    course.push_back(destination);
    do{
        course.push_back(g.nodes[final].parent);
        final = g.nodes[final].parent;
    }while(final != source);
    reverse(course.begin(), course.end());
    for(int station = 0; station < course.size(); station++){
        if(station == course.size() -1)
            cout << course[station];
        else
            cout << course[station] << " -> ";
    }
    return 0;
}
