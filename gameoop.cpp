#include "gameoop.h"
using namespace std;
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    bool running = true;
    int choice;
    Kingdom* kingdom = nullptr;

    while (running) {
        clearScreen();
        printStrongholdBanner();
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: 
        {
            string kingdomName;
            cout << "Enter a name for your kingdom: ";
            cin.ignore();
            getline(cin, kingdomName);

            delete kingdom;
            kingdom = new Kingdom(kingdomName);

            cout << "\nWelcome to the Kingdom of " << kingdomName << "!\n";
            waitKey();

            playGame(kingdom);
        }
        break;

        case 2: 
        {
            string filename;
            cout << "Enter save file name: ";
            cin >> filename;

            if (kingdom == nullptr) {
                kingdom = new Kingdom("Loaded Kingdom");
            }

            if (kingdom->loadGame(filename + ".sav")) {
                cout << "Game loaded successfully!\n";
                waitKey();
                playGame(kingdom);
            }
            else {
                cout << "Failed to load game.\n";
                waitKey();
            }
        }
        break;

        case 3: 
        {
            clearScreen();
            GameIO gameIO;
            gameIO.displayHighScores();
            waitKey();
        }
        break;

        case 4: 
            cout << "Thank you for playing Stronghold!\n";
            running = false;
            break;

        default:
            cout << "Invalid choice.\n";
            waitKey();
            break;
        }
    }

    
    delete kingdom;

    return 0;
}