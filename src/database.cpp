#include "database.h"
#include <iostream>

Database::Database(const std::string& db_path) : db_(nullptr), db_path_(db_path) {}

Database::~Database() {
    if (db_) sqlite3_close(db_);
}

bool Database::initialize() {
    if (sqlite3_open(db_path_.c_str(), &db_) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS flights (
            id              INTEGER PRIMARY KEY AUTOINCREMENT,
            flight_number   TEXT NOT NULL,
            origin          TEXT NOT NULL,
            destination     TEXT NOT NULL,
            departure_time  TEXT NOT NULL,
            arrival_time    TEXT NOT NULL,
            status          TEXT NOT NULL DEFAULT 'ON_TIME',
            delay_minutes   INTEGER DEFAULT 0
        );
    )";

    return executeSQL(sql);
}

bool Database::executeSQL(const std::string& sql) {
    char* err_msg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "SQL error: " << err_msg << "\n";
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool Database::addFlight(const Flight& flight) {
    std::string sql =
        "INSERT INTO flights (flight_number, origin, destination, "
        "departure_time, arrival_time, status, delay_minutes) VALUES ('"
        + flight.flight_number + "', '"
        + flight.origin + "', '"
        + flight.destination + "', '"
        + flight.departure_time + "', '"
        + flight.arrival_time + "', '"
        + flight.status + "', "
        + std::to_string(flight.delay_minutes) + ");";
    return executeSQL(sql);
}

bool Database::deleteFlight(int id) {
    return executeSQL("DELETE FROM flights WHERE id = " + std::to_string(id) + ";");
}

std::vector<Flight> Database::getAllFlights() {
    std::vector<Flight> flights;
    std::string sql = "SELECT * FROM flights ORDER BY departure_time;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return flights;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Flight f;
        f.id             = sqlite3_column_int(stmt, 0);
        f.flight_number  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        f.origin         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        f.destination    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        f.departure_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        f.arrival_time   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        f.status         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        f.delay_minutes  = sqlite3_column_int(stmt, 7);
        flights.push_back(f);
    }

    sqlite3_finalize(stmt);
    return flights;
}

std::vector<Flight> Database::getFlightsByStatus(const std::string& status) {
    std::vector<Flight> flights;
    std::string sql = "SELECT * FROM flights WHERE status = '" + status + "' ORDER BY departure_time;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return flights;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Flight f;
        f.id             = sqlite3_column_int(stmt, 0);
        f.flight_number  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        f.origin         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        f.destination    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        f.departure_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        f.arrival_time   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        f.status         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        f.delay_minutes  = sqlite3_column_int(stmt, 7);
        flights.push_back(f);
    }

    sqlite3_finalize(stmt);
    return flights;
}

std::vector<Flight> Database::getFlightsByOrigin(const std::string& origin) {
    std::vector<Flight> flights;
    std::string sql = "SELECT * FROM flights WHERE origin = '" + origin + "' ORDER BY departure_time;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return flights;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Flight f;
        f.id             = sqlite3_column_int(stmt, 0);
        f.flight_number  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        f.origin         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        f.destination    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        f.departure_time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        f.arrival_time   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        f.status         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        f.delay_minutes  = sqlite3_column_int(stmt, 7);
        flights.push_back(f);
    }

    sqlite3_finalize(stmt);
    return flights;
}

int Database::getTotalFlights() {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(*) FROM flights;";
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return 0;
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
}

double Database::getAverageDelay() {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT AVG(delay_minutes) FROM flights WHERE status = 'DELAYED';";
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return 0.0;
    double avg = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        avg = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return avg;
}

std::vector<std::pair<std::string, int>> Database::getTopRoutes(int limit) {
    std::vector<std::pair<std::string, int>> routes;
    std::string sql =
        "SELECT origin || ' -> ' || destination AS route, COUNT(*) as cnt "
        "FROM flights GROUP BY origin, destination "
        "ORDER BY cnt DESC LIMIT " + std::to_string(limit) + ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return routes;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string route = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int count = sqlite3_column_int(stmt, 1);
        routes.push_back({route, count});
    }

    sqlite3_finalize(stmt);
    return routes;
}
