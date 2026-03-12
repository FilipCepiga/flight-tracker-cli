#include "menu.h"
#include <iostream>
#include <iomanip>
#include <limits>

Menu::Menu(Database& database) : db_(database) {}

void Menu::run() {
    int choice = 0;
    do {
        showMainMenu();
        choice = getIntInput("Your choice");
        switch (choice) {
            case 1: showAllFlights();    break;
            case 2: addFlightPrompt();   break;
            case 3: searchFlights();     break;
            case 4: showStatistics();    break;
            case 5: deleteFlightPrompt(); break;
            case 0: std::cout << "\nGoodbye!\n"; break;
            default: std::cout << "\nInvalid option. Try again.\n";
        }
    } while (choice != 0);
}

void Menu::showMainMenu() {
    printSeparator();
    std::cout << "   ✈  FLIGHT TRACKER CLI\n";
    printSeparator();
    std::cout << "  [1] Show all flights\n";
    std::cout << "  [2] Add new flight\n";
    std::cout << "  [3] Search flights\n";
    std::cout << "  [4] Statistics\n";
    std::cout << "  [5] Delete flight\n";
    std::cout << "  [0] Exit\n";
    printSeparator();
}

void Menu::printSeparator() {
    std::cout << "----------------------------------------\n";
}

void Menu::printFlight(const Flight& f) {
    printSeparator();
    std::cout << "  ID:          " << f.id << "\n";
    std::cout << "  Flight:      " << f.flight_number << "\n";
    std::cout << "  Route:       " << f.origin << " -> " << f.destination << "\n";
    std::cout << "  Departure:   " << f.departure_time << "\n";
    std::cout << "  Arrival:     " << f.arrival_time << "\n";
    std::cout << "  Status:      " << f.status << "\n";
    if (f.delay_minutes > 0)
        std::cout << "  Delay:       " << f.delay_minutes << " min\n";
}

void Menu::showAllFlights() {
    auto flights = db_.getAllFlights();
    if (flights.empty()) {
        std::cout << "\nNo flights in database.\n";
        return;
    }
    std::cout << "\nTotal flights: " << flights.size() << "\n";
    for (const auto& f : flights)
        printFlight(f);
    printSeparator();
}

void Menu::addFlightPrompt() {
    std::cout << "\n--- ADD NEW FLIGHT ---\n";
    Flight f;
    f.id = 0;
    f.flight_number  = getStringInput("Flight number (e.g. LO123)");
    f.origin         = getStringInput("Origin airport code (e.g. WAW)");
    f.destination    = getStringInput("Destination airport code (e.g. LHR)");
    f.departure_time = getStringInput("Departure time (YYYY-MM-DD HH:MM)");
    f.arrival_time   = getStringInput("Arrival time   (YYYY-MM-DD HH:MM)");
    f.status         = getStringInput("Status (ON_TIME / DELAYED / CANCELLED)");
    f.delay_minutes  = getIntInput("Delay in minutes (0 if none)");

    if (db_.addFlight(f))
        std::cout << "\n✓ Flight added successfully!\n";
    else
        std::cout << "\n✗ Failed to add flight.\n";
}

void Menu::searchFlights() {
    std::cout << "\n--- SEARCH FLIGHTS ---\n";
    std::cout << "  [1] By status\n";
    std::cout << "  [2] By origin airport\n";
    int choice = getIntInput("Your choice");

    std::vector<Flight> results;
    if (choice == 1) {
        std::string status = getStringInput("Status (ON_TIME / DELAYED / CANCELLED)");
        results = db_.getFlightsByStatus(status);
    } else if (choice == 2) {
        std::string origin = getStringInput("Origin airport code (e.g. WAW)");
        results = db_.getFlightsByOrigin(origin);
    } else {
        std::cout << "Invalid option.\n";
        return;
    }

    if (results.empty()) {
        std::cout << "\nNo flights found.\n";
        return;
    }
    std::cout << "\nFound: " << results.size() << " flight(s)\n";
    for (const auto& f : results)
        printFlight(f);
    printSeparator();
}

void Menu::showStatistics() {
    std::cout << "\n--- STATISTICS ---\n";
    printSeparator();
    std::cout << "  Total flights:     " << db_.getTotalFlights() << "\n";
    std::cout << "  Avg delay:         " << std::fixed << std::setprecision(1)
              << db_.getAverageDelay() << " min\n";

    std::cout << "\n  Top routes:\n";
    auto routes = db_.getTopRoutes(5);
    int rank = 1;
    for (const auto& [route, count] : routes)
        std::cout << "  " << rank++ << ". " << route << " (" << count << " flights)\n";
    printSeparator();
}

void Menu::deleteFlightPrompt() {
    std::cout << "\n--- DELETE FLIGHT ---\n";
    int id = getIntInput("Enter flight ID to delete");
    if (db_.deleteFlight(id))
        std::cout << "\n✓ Flight deleted.\n";
    else
        std::cout << "\n✗ Failed to delete flight.\n";
}

int Menu::getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt << ": ";
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string Menu::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt << ": ";
    std::getline(std::cin, value);
    return value;
}
