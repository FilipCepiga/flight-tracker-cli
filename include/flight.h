#pragma once

#include <string>

struct Flight {
    int id;
    std::string flight_number;   // np. "LO123"
    std::string origin;          // kod lotniska np. "WAW"
    std::string destination;     // kod lotniska np. "LHR"
    std::string departure_time;  // format "YYYY-MM-DD HH:MM"
    std::string arrival_time;    // format "YYYY-MM-DD HH:MM"
    std::string status;          // "ON_TIME", "DELAYED", "CANCELLED"
    int delay_minutes;           // 0 jeśli brak opóźnienia
};

struct Airport {
    std::string code;   // np. "WAW"
    std::string name;   // np. "Warsaw Chopin Airport"
    std::string city;
    std::string country;
};
