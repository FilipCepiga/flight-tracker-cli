# ✈️ Flight Tracker CLI

A command-line application written in **C++17** for tracking flights and airline operations, built with **SQLite** for persistent data storage.

Inspired by real-world airline operations systems that track millions of flights and support crew management across global alliances.

---

## Features

- Add, view, and delete flights
- Search flights by status or origin airport
- View operational statistics: total flights, average delays, top routes
- Persistent storage via embedded SQLite database
- Clean CLI interface

---

## Tech Stack

| Technology | Purpose |
|------------|---------|
| C++17 | Core application logic |
| SQLite3 | Embedded relational database |
| CMake 3.15+ | Build system |

---

## Project Structure
```
flight-tracker-cli/
├── CMakeLists.txt
├── include/
│   ├── flight.h       # Flight and Airport data structures
│   ├── database.h     # Database class interface
│   └── menu.h         # CLI menu interface
└── src/
    ├── main.cpp       # Entry point
    ├── flight.cpp     # Flight helper functions
    ├── database.cpp   # SQLite operations (CRUD + stats)
    └── menu.cpp       # User interface logic
```

---

## Building & Running

### Prerequisites
- C++17 compatible compiler (g++ / clang++)
- CMake 3.15+
- SQLite3 development libraries

### Build
```bash
mkdir build && cd build
cmake ..
make
./flight_tracker
```

---

## Sample Usage
```
----------------------------------------
   ✈  FLIGHT TRACKER CLI
----------------------------------------
  [1] Show all flights
  [2] Add new flight
  [3] Search flights
  [4] Statistics
  [5] Delete flight
  [0] Exit
----------------------------------------

--- STATISTICS ---
----------------------------------------
  Total flights:     12
  Avg delay:         34.5 min

  Top routes:
  1. WAW -> LHR (4 flights)
  2. KRK -> CDG (3 flights)
  3. WAW -> JFK (2 flights)
----------------------------------------
```

---

## SQL Schema
```sql
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
```

---

## Author

Built as a portfolio project demonstrating C++ and SQL skills in the context of airline operations software.
