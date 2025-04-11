#include "delhi/delhi.h"
#include "bangalore/bangalore.h"
#include "hyderabad/hyderabad.h"
#include "inter-city/inter-city.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Starting initialization...\n";

    std::cout << "Initializing Delhi graphs...\n";
    delhi::init_delhi_graphs();
    std::cout << "Delhi graphs initialized.\n";

    std::cout << "Initializing Bangalore graphs...\n";
    bangalore::init_bangalore_graphs();
    std::cout << "Bangalore graphs initialized.\n";

    std::cout << "Initializing Hyderabad graphs...\n";
    hyderabad::init_hyderabad_graphs();
    std::cout << "Hyderabad graphs initialized.\n";

    std::cout << "Initializing Inter-city graphs...\n";
    intercity::init_intercity_graphs();
    std::cout << "Inter-city graphs initialized.\n";

    int travel_type;
    std::string source, dest;
    int choice;

    std::cout << "Welcome to Subway Navigation System\n";
    std::cout << "1. Intra-city travel\n2. Inter-city travel\nEnter travel type: ";
    std::cin >> travel_type;

    if (travel_type == 1) {
        std::string city;
        std::cout << "Enter city (Delhi, Bangalore, Hyderabad): ";
        std::cin >> city;
        std::cout << "Enter source station: ";
        std::cin.ignore();
        std::getline(std::cin, source);
        std::cout << "Enter destination station: ";
        std::getline(std::cin, dest);
        std::cout << "1. Shortest time\n2. Minimum fare\n3. Fewest interchanges\nEnter choice: ";
        std::cin >> choice;

        if (city == "Delhi") {
            delhi::find_delhi_route(source, dest, choice);
        } else if (city == "Bangalore") {
            bangalore::find_bangalore_route(source, dest, choice);
        } else if (city == "Hyderabad") {
            hyderabad::find_hyderabad_route(source, dest, choice);
        } else {
            std::cout << "Invalid city\n";
        }
    } else if (travel_type == 2) {
        std::cout << "Enter source (City_Station, e.g., Delhi_New Delhi): ";
        std::cin.ignore();
        std::getline(std::cin, source);
        std::cout << "Enter destination (City_Station): ";
        std::getline(std::cin, dest);
        std::cout << "1. Shortest time\n2. Minimum fare\n3. Fewest interchanges\nEnter choice: ";
        std::cin >> choice;

        intercity::find_intercity_route(source, dest, choice);
    } else {
        std::cout << "Invalid travel type\n";
    }

    return 0;
}