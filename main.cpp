#include <iostream>
#include <string>
#include "delhi/delhi.h"
#include "hyderabad/hyderabad.h"
#include "bangalore/bangalore.h"
#include "inter-city/inter-city.h"

using namespace std;

int main() {
    char continue_choice;

    // Initialize graphs for all cities and inter-city at startup
    delhi::init_delhi_graphs();
    hyderabad::init_hyderabad_graphs();
    bangalore::init_bangalore_graphs();
    intercity::init_intercity_graphs();

    do {
        int travel_type, city, choice;
        string src, dest;

        cout << "Welcome to the Subway Navigation System\n";
        cout << "1. Intra-city travel\n";
        cout << "2. Inter-city travel\n";
        cout << "Give Input: ";
        cin >> travel_type;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nInvalid input, please enter a number.\n";
            continue;
        }

        if (travel_type == 1) {
            cout << "Choose city:\n";
            cout << "1. Delhi\n";
            cout << "2. Hyderabad\n";
            cout << "3. Bangalore\n";
            cout << "Enter choice (1-3): ";  // Clarified prompt
            cin >> city;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nInvalid city choice, please enter a number between 1 and 3.\n";
                continue;
            }

            cout << "Enter source station: ";
            cin.ignore();  // Clear leftover newline
            getline(cin, src);
            cout << "Enter destination station: ";
            getline(cin, dest);

            cout << "1. Shortest travel time\n";
            cout << "2. Minimum fare cost\n";
            cout << "3. Fewest interchanges\n";
            cout << "Give your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nInvalid choice, please enter a number between 1 and 3.\n";
                continue;
            }

            if (city == 1) {
                delhi::find_delhi_route(src, dest, choice);
            } else if (city == 2) {
                hyderabad::find_hyderabad_route(src, dest, choice);
            } else if (city == 3) {
                bangalore::find_bangalore_route(src, dest, choice);
            } else {
                cout << "\nInvalid city choice\n";
            }
        } else if (travel_type == 2) {
            cout << "Enter source station (e.g., Delhi_New Delhi): ";
            cin.ignore();
            getline(cin, src);
            cout << "Enter destination station (e.g., Hyderabad_Secunderabad): ";
            getline(cin, dest);

            cout << "1. Shortest travel time\n";
            cout << "2. Minimum fare cost\n";
            cout << "3. Fewest interchanges\n";
            cout << "Give your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nInvalid choice, please enter a number between 1 and 3.\n";
                continue;
            }

            intercity::find_intercity_route(src, dest, choice);
        } else {
            cout << "\nInvalid travel type\n";
        }

        cout << "Do you want to perform another operation? (Y/N): ";
        cin >> continue_choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue_choice = 'N';  // Default to exit on invalid input
        }
        cin.ignore();  // Clear leftover newline

    } while (continue_choice == 'Y' || continue_choice == 'y');

    cout << "\nThank you for using the Subway Navigation System. Goodbye!\n";
    return 0;
}