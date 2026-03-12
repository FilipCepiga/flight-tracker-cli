#pragma once

#include "database.h"

class Menu {
public:
    Menu(Database& database);
    void run();

private:
    Database& db_;

    void showMainMenu();
    void showAllFlights();
    void addFlightPrompt();
    void searchFlights();
    void showStatistics();
    void deleteFlightPrompt();

    void printFlight(const Flight& flight);
    void printSeparator();
    int getIntInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
};
