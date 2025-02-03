#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <map>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <vector>

using namespace std;


void setConsolePosition(int x, int y) {
    HWND consoleWindow = GetConsoleWindow(); // Get the handle to the console window
    if (consoleWindow != nullptr) {
        MoveWindow(consoleWindow, x, y, 80, 10, TRUE); // Set position (x, y) and size (width: 800, height: 600)
    } else {
        std::cerr << "Failed to get console window handle!" << std::endl;
    }
}

void enableAnsiEscapeCodes() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hConsole == INVALID_HANDLE_VALUE) return;
    if (!GetConsoleMode(hConsole, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
}


// Function to get the current time in Finnish time (EET/EEST)
tm getCurrentFinnishTime() {
    time_t now = time(nullptr);
    tm localTime = *localtime(&now);
    return localTime;
}

// Function to check if the current time is within the opening hours
bool isOpen(int openHour, int openMinute, int closeHour, int closeMinute, int currentHour, int currentMinute) {
    if (currentHour > openHour && currentHour < closeHour) {
        return true;
    } else if (currentHour == openHour && currentMinute >= openMinute) {
        return true;
    } else if (currentHour == closeHour && currentMinute < closeMinute) {
        return true;
    }
    return false;
}

// ANSI escape codes for colors
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string RESET = "\033[0m";

int main() {
     setConsolePosition(1200, -300);
    system("mode con: cols=80 lines=10");
    enableAnsiEscapeCodes();
    system("cls");
    
    vector<pair<string, pair<pair<int, int>, pair<int, int>>>> stockExchanges = {
        {"Tokyo Stock Exchange (TSE)", {{2, 0}, {9, 0}}},
        {"Sydney Stock Exchange (SSX)", {{2, 0}, {8, 0}}},
        {"Hong Kong Stock Exchange (HKEX)", {{3, 0}, {10, 0}}},
        {"London Stock Exchange (LSE)", {{10, 0}, {18, 0}}},
        {"New York Stock Exchange (NYSE)", {{16, 30}, {23, 0}}},
        {"NASDAQ", {{16, 30}, {23, 0}}}
    };

    while (true) {
        // Clear the console
        cout << "\033[H";

        // Get the current Finnish time
        tm currentTime = getCurrentFinnishTime();
        int currentHour = currentTime.tm_hour;
        int currentMinute = currentTime.tm_min;
        int currentSecond = currentTime.tm_sec;

        // Display current time in hh:mm:ss format
        cout << "Current Time: "
            << setfill('0') << setw(2) << currentHour << ":"
            << setw(2) << currentMinute << ":"
            << setw(2) << currentSecond << endl;


        cout << "Stock Exchange Status (Finnish Time)\n";
        cout << "--------------------------------\n";

        // Display stock exchange information
        for (const auto& exchange : stockExchanges) {
            string name = exchange.first;
            int openHour = exchange.second.first.first;
            int openMinute = exchange.second.first.second;
            int closeHour = exchange.second.second.first;
            int closeMinute = exchange.second.second.second;
            bool open = isOpen(openHour, openMinute, closeHour, closeMinute, currentHour, currentMinute);

       

            // Set a fixed width for the name (e.g., 40 characters) without affecting time formatting
            cout << left << setw(33) << setfill(' ') << name << "Opening Hours: "
                << right << setw(2) << setfill('0')  << openHour  << ":" << setw(2) << openMinute
                << " - "
                << setw(2) << closeHour << ":" << setw(2) << closeMinute
                << " Status: ";

            if (open) {
                cout << GREEN << "Open" << RESET;
            } else {
                cout << RED << "Closed" << RESET;
            }

            cout << endl;
        }
        cout << "\r";
        // Wait for 100ms before updating
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
