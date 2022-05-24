#include "../include/Appstarter.h"

#include <iostream>

AppStarter::AppStarter() {
    menuStack.push(new MainMenu(app));
}

void AppStarter::start() {
    app.setGraph("10");
    while (!menuStack.empty()) {
        menuStack.top()->display();
        Menu *nextMenu = menuStack.top()->nextMenu();
        if (nextMenu){
            if (nextMenu != menuStack.top()){
                menuStack.push(nextMenu);
            }
        }
        else {
            delete menuStack.top();
            menuStack.pop();
        }
    }
    //app.saveData();
}