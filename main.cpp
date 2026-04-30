#include "Type.h"
#include "Character.h"
#include <iostream>
#include <string>
#include <random>

// colors for text output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace std;

int main()
{
    int choice;

    cout << "CE222 Final Project by Troy Lagasse, Cody Overgaard, Carlos Rodriguez" << endl;
    cout << GREEN << "1. Start Game" << RESET << endl;
    cout << RED << "2. Quit Game" << RESET << endl;
    cout << "Choice: ";
    cin >> choice;

    if (choice != 1)
    {
        cout << "\nQuitting Game!" << endl;
        return 0;
    }

    while (true) {

        Character* player;
        Character* enemy;
        int strengthPotAmount;
        int healthPotAmount;
        int enemyHealthPotAmount;
        int enemyStrengthPotAmount;

        int diffChoice;
        cout << "\nChoose Difficulty" << endl;
        cout << GREEN << "1. Easy" << RESET << endl;
        cout << YELLOW << "2. Medium" << RESET << endl;
        cout << RED << "3. Hard" << RESET << endl;
        cout << "Choice:";
        cin >> diffChoice;

        if (diffChoice == 1)
        {
            strengthPotAmount = 5;
            healthPotAmount = 5;
            enemyHealthPotAmount = 1;
            enemyStrengthPotAmount = 1;
            enemy = new Light();
        }
        else if (diffChoice == 2)
        {
            strengthPotAmount = 3;
            healthPotAmount = 3;
            enemyHealthPotAmount = 3;
            enemyStrengthPotAmount = 3;
            enemy = new Medium();
        }
        else if (diffChoice == 3)
        {
            strengthPotAmount = 0;
            healthPotAmount = 1;
            enemyHealthPotAmount = 5;
            enemyStrengthPotAmount = 5;
            enemy = new Heavy();
        }
        else
        {
            cout << RED << "Invalid Choice!" << RESET << endl;
            continue;
        }

        bool quitGame = false;

        // choose character type
        cout << "\nChoose your character type: " << endl;
        cout << CYAN << "1. Light (Inflict High Damage, Low Health || Special Move: DOUBLE STRIKE)" << RESET << endl;
        cout << GREEN << "2. Medium (Balanced Damage and Health || Special Move: LIFE STEAL)" << RESET << endl;
        cout << BLUE << "3. Heavy (Inflict Low Damage, High Health || Special Move: FORTIFY)" << RESET << endl;
        cout << "Choice:";
        cin >> choice;

        if (choice == 1)
            player = new Light();
        else if (choice == 2)
            player = new Medium();
        else
            player = new Heavy();

        player->setLabel("You");
        enemy->setLabel("Enemy");

        cout << "\nYou chose: " << player->getType() << endl;
        cout << "\nEnemy chose: " << enemy->getType() << endl;

        while (player->isAlive() && enemy->isAlive())
        {
            int action;

            cout << "\nChoose an action:" << endl;
            cout << "1. Attack" << endl;
            cout << "2. Use Potion" << endl;
            if (player->getSpecialCooldown() != 0)
                cout << "3. Special Move (Cooldown: " << player->getSpecialCooldown() << " turns)" << endl;
            else
                cout << "3. Special Move (SPECIAL READY!!)" << endl;
            cout << "4. Quit" << endl;
            cout << "Choice: ";
            cin >> action;

            if (action == 1)
            {
                player->attackTarget(*enemy);
                cout << RED << "Enemy HP: " << enemy->getHealth() << RESET << endl;
            }
            else if (action == 2)
            {
                int potionSelect;

                cout << "\n1. Heal (" << healthPotAmount << ")" << endl;
                cout << "2. Strength (" << strengthPotAmount << ")" << endl;
                cin >> potionSelect;

                if (potionSelect == 1 && healthPotAmount > 0) {
                    player->heal();
                    healthPotAmount--;
                }
                else if (potionSelect == 2 && strengthPotAmount > 0) {
                    player->strength();
                    strengthPotAmount--;
                }
                else {
                    cout << RED << "Invalid or no potions left." << RESET << endl;
                }
            }
            else if (action == 3)
            {
                player->specialMove(*enemy);
                cout << RED << "Enemy HP: " << enemy->getHealth() << RESET << endl;
            }
            else if (action == 4) {
                quitGame = true;
                break;
            }

            if (!enemy->isAlive())
                break;

            cout << "\nEnemy Turn!" << endl;

            uniform_int_distribution<int> ai(0,2);
            int enemyAction = ai(gen);

            if (enemyAction == 0) {
                cout << "Enemy Attacks!" << endl;
                enemy->attackTarget(*player);
            }
            else if (enemyAction == 1 && (enemyHealthPotAmount > 0 || enemyStrengthPotAmount > 0)) {

                uniform_int_distribution<int> potion(0,1);
                int potionChoice = potion(gen);

                if (potionChoice == 0 && enemyHealthPotAmount > 0) {
                    enemy->heal();
                    enemyHealthPotAmount--;
                }
                else if (potionChoice == 1 && enemyStrengthPotAmount > 0) {
                    enemy->strength();
                    enemyStrengthPotAmount--;
                }
                else {
                    cout << "Enemy attacks instead!" << endl;
                    enemy->attackTarget(*player);
                }
            }
            else if (enemyAction == 2) {

                if (enemy->getSpecialCooldown() == 0) {
                    cout << CYAN << "Enemy uses SPECIAL!" << RESET << endl;
                    enemy->specialMove(*player);
                }
                else {
                    cout << "Enemy attacks!" << endl;
                    enemy->attackTarget(*player);
                }
            }

            cout << BLUE << "Player HP: " << player->getHealth() << RESET << endl;

            player->updateEffects();
            enemy->updateEffects();
        }

        if (quitGame) {
            delete player;
            delete enemy;
            break;
        }

        if (player->isAlive())
            cout << YELLOW << "\nYou Win!" << RESET << endl;
        else
            cout << RED << "\nYou Lose!" << RESET << endl;

        delete player;
        delete enemy;

        int again;
        cout << "1. Play Again" << endl;
        cout << "2. Quit" << endl;
        cout << "Choice:";
        cin >> again;

        if (again != 1)
            break;
    }

    return 0;
}