#include <iostream>
#include "Atomic_Chaos/AtomicChaosApp.h"
#include "Orbital_Chaos/OrbitalChaosApp.h"

bool getIntInput(int& val) {
    std::cin >> val;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter an integer.\n";
        return false;
    }
    return true;
}

int main()
{
    using namespace std;
    int choice;

    do {
        cout << "\n=== 2D Physics Engine ===\n";
        cout << "1. Atomic Chaos (Particle Collision)\n";
        cout << "2. Orbital Chaos (Gravity Simulation)\n";
        cout << "3. Exit \n";
        cout << "Choose simulation : ";

        if (!getIntInput(choice)) continue;

        switch (choice) {
        case 1:{
            AtomicChaosApp app;
            app.initialize();
            app.run();
            app.cleanup();
            break;
        }
        case 2: {
            OrbitalChaosApp app;
            app.initialize();
            app.run();
            app.cleanup();
            break;
        }
        case 3: {
            cout << "\nThis Was 2D Physics Simulation - Thanks :)" << endl;
            break;
        }
        default: {
            cout << "Invalid choice, try again." << endl;
            break;
            }
        }
    } while (choice != 3);
    return 0;
}