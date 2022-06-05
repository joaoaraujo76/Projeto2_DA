#include "../include/Menu.h"

#include <iostream>

using namespace std;

Menu::~Menu() = default;

Menu::Menu(App &app): app(app) {}

int Menu::readInt() {
    int opt; string optstr;
    while (true){
        stringstream ss;
        ss << readStr(); ss >> opt;
        if(!ss.fail() && ss.eof() && opt>=0) break;
        cout << "Invalid input. Try a valid integer..." << endl;
    }
    cout << endl;
    return opt;
}

void Menu::waitForKey() {
    string str;
    cout << endl << "Press enter to continue..." << endl;
    getline(cin, str);
}

std::string Menu::readStr() {
    string str;
    cout << ": ";
    getline(cin, str);
    return str;
}

Menu *Menu::invalidInput() {
    cout << "Invalid input option!" << endl;
    return this;
}

MainMenu::MainMenu(App &app): Menu(app){}

void MainMenu::display(){
    cout << endl;
    cout << "Main Menu:" << endl;
    cout << "1 - First Scenario - groups that don't split" << endl;
    cout << "2 - Second Scenario - groups that can split" << endl;
    cout << "3 - Settings Menu" << endl;
    cout << "0 - Exit" << endl;
    cout << endl;
}

Menu *MainMenu::nextMenu() {
    switch (readInt()) {
        case 1:
            return new FirstScenarioMenu(app);
        case 2:
            return new SecondScenarioMenu(app);
        case 3:
            return new SettingsMenu(app);
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

FirstScenarioMenu::FirstScenarioMenu(App &app): Menu(app){}

void FirstScenarioMenu::display(){
    cout << endl;
    cout << "First Scenario Menu:" << endl;
    cout << "1 - Maximize group dimension (1.1)"<< endl;
    cout << "2 - Maximize group dimension and minimize number of bus switches (1.2)"<< endl;
    cout << "0 - Exit" << endl;
    cout << endl;
}

Menu *FirstScenarioMenu::nextMenu() {
    switch (readInt()) {
        case 1:{
            do{
                cout << "Please insert the source station: ";
                source = readInt();
                cout << "Please insert the destination station: ";
                dest = readInt();
                if(source == dest){
                    cout << "Stations must be different: " << endl << endl;
                    source = 0; dest = 0;
                }
                capacity = app.maximumCapacityPath(source, dest);
                if(capacity == -1)
                    cout << "Insert valid Station numbers" << endl;
                if (capacity == -2)
                    cout << "No path between source and destination" << endl;
            } while (capacity < 0);
            return new Scenario1_1(app, capacity, source, dest);
        }
        case 2:{
            bool done = false;
            while (!done){
                cout << "Please insert the source station: ";
                source = readInt();
                cout << "Please insert the destination station: ";
                dest = readInt();
                if(source == dest){
                    cout << "Stations must be different " << endl << endl;
                    continue;
                }
                if (source <= 0 || source > app.getGraph().getNumNodes() || dest <= 0 || dest > app.getGraph().getNumNodes()) {
                    cout << "Insert valid Station numbers" << endl;
                    continue;
                }
                app.paretoPaths.clear();
                app.edmondsKarp1(source, dest);
                app.maximumCapacityPath(source, dest);
                vector <int> maxCapPath = app.getPathScenario_1(source, dest);
                pair<vector<int>, int> pair = {maxCapPath, app.getGraph().getNodes()[dest].maxCap};
                app.paretoPaths.push_back(pair);
                app.filterPaths1_2();
                if (app.paretoPaths.empty()){
                    cout << "No path between source and destination" << endl;
                    continue;
                }
                done = true;
            }
            return new Scenario1_2(app);
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario1_1::Scenario1_1(App &app, int capacity, int source, int dest) : Menu(app) {
    this->capacity = capacity; this->source = source; this->dest = dest;
}

void Scenario1_1::display() {
    cout << endl;
    cout << "Scenario 1.1" << endl;
    cout << "1 - View Path"<<endl;
    cout << "0 - Exit";
    cout << endl << endl;
    cout << "Maximum capacity: " << capacity << endl;

}


Menu *Scenario1_1::nextMenu() {
    switch (readInt()) {
        case 1: {
            vector<int> stops = app.getPathScenario_1(source, dest);
            for(int stop = 0; stop < stops.size(); stop++){
                if(stop == stops.size() -1)
                    cout << stops[stop];
                else
                    cout << stops[stop] << " -> ";
            }
            waitForKey();
            return this;
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario1_2::Scenario1_2(App &app) : Menu(app) {

}

void Scenario1_2::display() {
    cout << endl;
    cout << "Scenario 1.2" << endl;
    cout << "1 - View Path"<<endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario1_2::nextMenu() {
    switch (readInt()) {
        case 1: {
            app.printPathScenario1_2();
            waitForKey();
            return this;
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

SecondScenarioMenu::SecondScenarioMenu(App &app): Menu(app){}

void SecondScenarioMenu::display(){
    cout << endl;
    cout << "Second Scenario Menu:" << endl;
    cout << "1 - Path for a group given his dimension (2.1)"<<endl;
    cout << "2 - Correct last path in 2.1 given a group size increase (2.2)"<<endl;
    cout << "3 - Check group max dimension (2.3)"<<endl;
    cout << "0 - Exit"<<endl;
    cout << endl;
}

Menu *SecondScenarioMenu::nextMenu() {
    switch (readInt()) {
        case 1:{
            done = false;
            while(!done){
                cout << "Insert source ";
                source = readInt();
                cout << "Insert destination ";
                dest = readInt();
                cout << "Insert group size ";
                capacity = readInt();
                if (source == dest) {
                    cout << "Source is the same as the destination" << endl;
                    continue;
                }
                if (source <= 0 || source > app.getGraph().getNumNodes() || dest <= 0 || dest > app.getGraph().getNumNodes()) {
                    cout << "Source or destination node aren't in the current graph" << endl;
                    continue;
                }
                done = true;
            }
            app.edmondsKarp2(source, dest, capacity, false, false);
            if(app.pathsMap.second < capacity) {
                cout << "The maximum group size of the path is " << app.pathsMap.second << endl;
                return this;
            }
            return new Scenario2_1(app);
        }
        case 2:{
            done = false;
            vector<int> lastInputInfo = app.lastPathInfo;
            while(!done) {
                source = lastInputInfo[0];
                dest = lastInputInfo[1];
                capacity = lastInputInfo[2];
                if (source == 0 || dest == 0) {
                    cout << "No path available to run Scenario 2.2, first run Scenario 2.1" << endl;
                    continue;
                }
                cout << "Insert group size increase for last the path of Scenario 2.1";
                int increase = readInt();
                if ((capacity + increase) <= app.pathsMap.second) {
                    cout << "The new group size is lower or equal than previous maximum capacity, so stills the same path" << endl;
                    continue;
                } else {
                    app.edmondsKarp2(source, dest, capacity + increase, true, false);
                }
                if (app.lastPathInfo[2] > app.pathsMap.second) {
                    cout << "The new group size is larger than the maximum capacity for the last trip" << endl;
                    cout << "Maximum capacity: " << app.pathsMap.second << endl;
                    app.lastPathInfo = lastInputInfo;
                    return this;
                }
                done = true;
            }
            cout << "Path capacity increased successfully" << endl;
            return new Scenario2_2(app);
        }
        case 3:{
            done = false;
            while(!done) {
                cout << "Insert source ";
                source = readInt();
                cout << "Insert destination ";
                dest = readInt();
                if (source == dest) {
                    cout << "Source is the same as the destination" << endl;
                    continue;
                }
                if (source <= 0 || source > app.getGraph().getNumNodes() || dest <= 0 || dest > app.getGraph().getNumNodes()) {
                    cout << "Source or destination node aren't in the current graph" << endl;
                    continue;
                }
                done = true;
            }
            app.edmondsKarp2(source, dest, -1, false, true);
            cout << "The maximum capacity of the path is " << app.pathsMap.second << endl;
            return new Scenario2_3(app);
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario2_1::Scenario2_1(App &app) : Menu(app) {

}

void Scenario2_1::display() {
    cout << endl;
    cout << "Scenario 2.1:" << endl;
    cout << "1 - View Path"<<endl;
    cout << "2 - Check when the group will get back together at the destination (at least) (2.4) " << endl;
    cout << "3 - Check the group's maximum waiting time during the path and when that occurs (2.5) " << endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario2_1::nextMenu() {
    switch (readInt()) {
        case 1:{
            cout << "The path: " << endl;
            app.printPathScenario_2();
            cout << endl;
            waitForKey();
            return this;
        }
        case 2:{
            return new Scenario2_4(app, app.getAuxGraph());
        }
        case 3:{
            return new Scenario2_5(app, app.getAuxGraph());
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario2_2::Scenario2_2(App &app) : Menu(app) {

}

void Scenario2_2::display() {
    cout << endl;
    cout << "Scenario 2.2:" << endl;
    cout << "1 - View Path"<<endl;
    cout << "2 - Check when the group will get back together at the destination (at least) (2.4) " << endl;
    cout << "3 - Check the group's maximum waiting time during the path and when that occurs (2.5) " << endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario2_2::nextMenu() {
    switch (readInt()) {
        case 1:{
            cout << "The path: " << endl;
            app.printPathScenario_2();
            cout << endl;
            waitForKey();
            return this;
        }
        case 2:{
            return new Scenario2_4(app, app.getAuxGraph());
        }
        case 3:{
            return new Scenario2_5(app, app.getAuxGraph());
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario2_3::Scenario2_3(App &app) : Menu(app) {

}

void Scenario2_3::display() {
    cout << endl;
    cout << "Scenario 2.3:" << endl;
    cout << "1 - View Path"<<endl;
    cout << "2 - Check when the group will get back together at the destination (at least) (2.4) " << endl;
    cout << "3 - Check the group's maximum waiting time during the path and when that occurs (2.5) " << endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario2_3::nextMenu() {
    switch (readInt()) {
        case 1:{
            cout << "The path: " << endl;
            app.printPathScenario_2();
            cout << endl;
            waitForKey();
            return this;
        }
        case 2:{
            return new Scenario2_4(app, app.getAuxGraph());
        }
        case 3:{
            return new Scenario2_5(app, app.getAuxGraph());
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario2_4::Scenario2_4(App &app, Graph &graph) : Menu(app) {
    this->graph = graph;

}

void Scenario2_4::display() {
    cout << endl;
    cout << "Scenario 2.4:" << endl;
    cout << "Group will get back together at destination in " << app.earliestStart(graph).first << " hour(s) (at least)" << endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario2_4::nextMenu() {
    switch (readInt()) {
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}

Scenario2_5::Scenario2_5(App &app, Graph &graph) : Menu(app) {
   info = app.maxWaitTime(graph);
}

void Scenario2_5::display() {
    cout << endl;
    cout << "Scenario 2.5:" << endl;
    cout << "Max time people have to wait: " << info.first << endl;
    cout << "1 - See Stations where that happens" << endl;
    cout << "0 - Exit";
    cout << endl;
}


Menu *Scenario2_5::nextMenu() {
    switch (readInt()) {
        case 1:{
            cout << "That happens in " << info.second.size() << " stations" << endl;
            if(!info.second.empty()){
                cout << "Stations nr: ";
                for(int i : info.second)
                    cout << i << " ";
            }
            cout << endl;
            return this;
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}
SettingsMenu::SettingsMenu(App &app): Menu(app){}

void SettingsMenu::display(){
    cout << endl;
    cout << "Settings Menu:" << endl;
    cout << "1 - Change current DataSet (curr: " << app.getGraphStr() << ")" << endl;
    cout << "0 - Exit" << endl;
    cout << endl;
}

Menu *SettingsMenu::nextMenu() {
    int graph;
    size_t string_size = 2;
    int precision;

    switch (readInt()) {
        case 1:{
            do{
                cout << "Please insert the new DataSet [1-10]: ";
                graph = readInt();
            } while (graph > 10 || graph < 1);
            precision = string_size - std::min(string_size, to_string(graph).size());
            app.setGraphStr(string(precision, '0').append(to_string(graph)));
            cout << "Done!" << endl;
            waitForKey();
            return this;
        }
        case 0:
            return nullptr;
        default:
            return invalidInput();
    }
}
