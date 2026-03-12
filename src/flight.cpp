#include "flight.h"
#include <iostream>
#include <iomanip>

std::string statusToString(const std::string& status) {
    if (status == "ON_TIME")   return "✓ On Time";
    if (status == "DELAYED")   return "⚠ Delayed";
    if (status == "CANCELLED") return "✗ Cancelled";
    return "Unknown";
}

std::string formatRoute(const Flight& f) {
    return f.origin + " → " + f.destination;
}

bool isValidStatus(const std::string& status) {
    return status == "ON_TIME" ||
           status == "DELAYED" ||
           status == "CANCELLED";
}

bool isValidAirportCode(const std::string& code) {
    // Kody lotnisk mają dokładnie 3 litery (np. WAW, LHR, JFK)
    if (code.length() != 3) return false;
    for (char c : code) {
        if (!isalpha(c)) return false;
    }
    return true;
}

bool isValidFlightNumber(const std::string& number) {
    // Format: 2 litery + 1-4 cyfry, np. LO123, BA7
    if (number.length() < 3 || number.length() > 6) return false;
    if (!isalpha(number[0]) || !isalpha(number[1])) return false;
    for (size_t i = 2; i < number.length(); i++) {
        if (!isdigit(number[i])) return false;
    }
    return true;
}
