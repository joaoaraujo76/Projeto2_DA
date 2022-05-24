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
        case 3: return new SettingsMenu(app);
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
    cout << "1 - Maximize group dimension"<< endl;
    cout << "2 - See path"<< endl;
    cout << "0 - Exit" << endl;
    cout << endl;
}

Menu *FirstScenarioMenu::nextMenu() {
    switch (readInt()) {
        case 1:{
            int capacity;
            bool condition = true;
            do{
                cout << "Please insert the source station: ";
                source = readInt();
                cout << "Please insert the destination station: ";
                destination = readInt();
                if(source == destination){
                    cout << "Stations must be different: " << endl << endl;
                    source = 0; destination = 0;
                }
                condition = ((capacity = app.maximumCapacityPath(source, destination)) == -1);
                if(condition)
                    cout << "Insert valid Station numbers" << endl;
            } while (condition);
            cout << "Maximum capacity: " << capacity << endl;
            waitForKey();
            return this;
        }
        case 2:{
            if(source == 0 && destination == 0){
                cout << "Must run any algorithm before!" << endl;
                return this;
            }
            vector<int> stops = app.getPath(source, destination);
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

SecondScenarioMenu::SecondScenarioMenu(App &app): Menu(app){}

void SecondScenarioMenu::display(){
    cout << endl;
    cout << "Settings Menu:" << endl;
    cout << "1 - Change current DataSet (curr: " << app.getGraph() << endl;
    cout << "0 - Exit" << endl;
    cout << endl;
}

Menu *SecondScenarioMenu::nextMenu() {
    int graph;
    switch (readInt()) {
        case 1:{
            do{
                cout << "Please insert the new working time: ";
                graph = readInt();
            } while (graph > 10 || graph < 1);
            app.setGraph(to_string(graph));
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












SettingsMenu::SettingsMenu(App &app): Menu(app){}

void SettingsMenu::display(){
    cout << endl;
    cout << "Settings Menu:" << endl;
    cout << "1 - Change current DataSet (curr: " << app.getGraph() << ")" <<  endl;
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
            app.setGraph(string(precision, '0').append(to_string(graph)));
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
