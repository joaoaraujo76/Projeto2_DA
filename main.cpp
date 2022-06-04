#include "include/Appstarter.h"

int main() {
    AppStarter app;
    app.start();
    return 0;
}

/*
int main() {
    App app;
    Graph graph(7, true);
    graph.addEdge(1, 2, 0, 5);
    graph.addEdge(1, 3, 0, 8);
    graph.addEdge(1, 4, 0, 10);
    graph.addEdge(2, 5, 0, 4);
    graph.addEdge(3, 5, 0, 0);
    graph.addEdge(3, 6, 0, 0);
    graph.addEdge(4, 6, 0, 2);
    graph.addEdge(4, 5, 0, 0);
    graph.addEdge(5, 7, 0, 7);
    graph.addEdge(6, 7, 0, 3);
    app.latestFinish(graph);
    return 0;
}
 */