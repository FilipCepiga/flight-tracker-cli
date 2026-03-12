#include <iostream>
#include "database.h"
#include "menu.h"

int main() {
    std::cout << "Initializing Flight Tracker...\n";

    Database db("flights.db");

    if (!db.initialize()) {
        std::cerr << "Failed to initialize database. Exiting.\n";
        return 1;
    }

    Menu menu(db);
    menu.run();

    return 0;
}
