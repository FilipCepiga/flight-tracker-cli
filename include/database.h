#pragma once

#include <string>
#include <vector>
#include "flight.h"
#include <sqlite3.h>

class Database {
public:
    Database(const std::string& db_path);
    ~Database();

    // Inicjalizacja tabel
    bool initialize();

    // Operacje na lotach
    bool addFlight(const Flight& flight);
    bool deleteFlight(int id);
    std::vector<Flight> getAllFlights();
    std::vector<Flight> getFlightsByStatus(const std::string& status);
    std::vector<Flight> getFlightsByOrigin(const std::string& origin);

    // Statystyki
    int getTotalFlights();
    double getAverageDelay();
    std::vector<std::pair<std::string, int>> getTopRoutes(int limit);

private:
    sqlite3* db_;
    std::string db_path_;

    bool executeSQL(const std::string& sql);
};
