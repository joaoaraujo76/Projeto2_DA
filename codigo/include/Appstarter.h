#ifndef PROJETO2_DA_APPSTARTER_H
#define PROJETO2_DA_APPSTARTER_H

#include "Menu.h"

class AppStarter {
    App app;
    std::stack<Menu*> menuStack;
public:
    /**
     * Default class constructor that pushes the main menu to the menuStack
     */
    AppStarter();
    /**
     * Starts the application loop and call the App methods to load and save data to files
     */
    void start();
};


#endif //PROJETO2_DA_APPSTARTER_H
