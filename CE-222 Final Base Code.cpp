#include<iostream>
#include<string>
#include<cstdlib> // for random number generator
#include<random>

// colors for text output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace std;



class Character { // base character class, to be inherited from
protected:
    int health;
    int attack;
    int defense;
    int strengthBonus = 0; // how much extra attack is active
    int strengthTurns = 0; // how many turns left;
    string type; // light, med, etc
    string label = "Player";
public:
    Character(string t, int h, int a, int d) : type(t), health(h), attack(a), defense(d) {}
    virtual ~Character() {}

    void setLabel(string l) {
        label = l;
    }

    virtual void attackTarget(Character& target) { // call this when attacking
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(-5, 10);
        int random = dist(gen); // a more maluable way to choose a random number
        if (rand() % 5 == 0)
        {
            cout << YELLOW << "Attack has missed!" << RESET << endl;
            return;
        }
        int damage = (attack + strengthBonus) - target.defense + random;
        if (damage < 1)
            damage = 1;
        target.health -= damage; // new health is health - damage done
        cout << "Attack did " << RED << damage << RESET << " damage!" << endl;
    }

    void heal() {
        health += 15;
        cout << GREEN << label << " healed 15 HP!" << RESET << endl;
    }

    void strength() {
        strengthBonus += 5;
        strengthTurns += 3;
        cout << MAGENTA << label << " gained +5 attack for 3 turns!" << RESET << endl;
    }

    void updateEffects()
    {
        if (strengthTurns > 0)
        {
            cout << "Strength for " << MAGENTA << strengthTurns - 1 << RESET << " remaining turns!!" << endl;
            strengthTurns--;
            if (strengthTurns == 0)
            {
                strengthBonus = 0;
                cout << MAGENTA << label << "Strength boost wore off!" << RESET << endl;
            }
        }
    }

    bool isAlive() {
        return health > 0; // is alive if health is above 0 
    }

    int getHealth() {
        return health;
    }

    string getType() {
        return type;
    }
};





// character types
class Light : public Character { // Light class, low health, high attack, low defense
public:
    Light() : Character("Light", 80, 18, 5) {}
};

class Medium : public Character { // Medium class, med health, med attack, med defense
public:
    Medium() : Character("Medium", 100, 14, 8) {}
};

class Heavy : public Character { // Heavy class, high health, low attack, high defense
public:
    Heavy() : Character("Heavy", 125, 11, 12) {}
};





int main()
{


    int choice;

    // starting screen
    cout << "CE222 Final Project by Troy Lagasse, Cody Overgaard,Carlos Rodriguez" << endl;
    cout << GREEN << "1. Start Game" << RESET << endl;
    cout << RED << "2. Quit Game" << RESET << endl;
    cout << "Choice: ";
    cin >> choice;

    if (choice != 1)
    {
        cout << "\nQuitting Game!" << endl;
        return 0;
    }

    while (true) { // outer loop

        Character* player;
        Character* enemy;
        int choice;
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

        // character selection 
        cout << "\nChoose your character type: " << endl;
        cout << "1. Light" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Heavy" << endl;
        cout << "Choice: ";
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

        // combat loop 
        while (player->isAlive() && enemy->isAlive()) {
            int action;

            cout << "\nChoose an action:" << endl;
            cout << "1. Attack" << endl;
            cout << "2. Use Potion" << endl;
            cout << "3. Quit" << endl;
            cout << "Choice: ";
            cin >> action;

            if (action == 1) {
                cout << "\nPlayer Attacks!" << endl;
                player->attackTarget(*enemy);
                cout << RED << "Enemy HP: " << enemy->getHealth() << RESET << endl;
            }
            else if (action == 2) {
                int potionSelect;

                cout << "\nSelect Potion Type to use: " << endl;
                cout << "1. Heal (" << healthPotAmount << ")" << endl;
                cout << "2. Strength (" << strengthPotAmount << ")" << endl;
                cout << "Choice: ";
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
            else if (action == 3) {
                cout << "Exiting game..." << endl;
                quitGame = true;
                break;
            }

            if (!enemy->isAlive())
                break;

            cout << "\nEnemy Turn!" << endl;

            int enemyAction = rand() % 2;

            if (enemyAction == 1 && (enemyHealthPotAmount > 0 || enemyStrengthPotAmount > 0)) {

                int potionChoice = rand() % 2;

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
            else {
                cout << "Enemy Attacks!" << endl;
                enemy->attackTarget(*player);
            }


            cout << "Player HP: " << player->getHealth() << endl;

            player->updateEffects();
            enemy->updateEffects();
        }

        // handles quit before results
        if (quitGame) {
            delete player;
            delete enemy;
            break; // exits outer loop
        }

        // results
        if (player->isAlive())
            cout << YELLOW << "\nYou Win!" << RESET << endl;
        else
            cout << RED << "\nYou Lose!" << RESET << endl;

        delete player;
        delete enemy;

        //play again prompt
        int again;
        cout << "1. Play Again" << endl;
        cout << "2. Quit Game" << endl;
        cout << "Choice: ";
        cin >> again;

        if (again != 1) {
            cout << "\nThanks for playing!" << endl;
            break; // exit outer loop
        }
    }

    return 0;
}
