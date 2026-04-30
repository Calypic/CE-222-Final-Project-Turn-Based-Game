#include<iostream>
#include<string>
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

// single RNG for entire program
mt19937 gen(random_device{}());

class Character {
protected:
    int health;
    int attack;
    int defense;
    int strengthBonus = 0;
    int strengthTurns = 0;
    int defenseBonus = 0;
    int defenseTurns = 0;
    int specialCooldown = 0;
    int maxCooldown = 6;
    string type;
    string label = "Player";

public:
    Character(string t, int h, int a, int d) : type(t), health(h), attack(a), defense(d) {}
    virtual ~Character() {}

    void setLabel(string l) {
        label = l;
    }

    virtual void attackTarget(Character& target) {
        uniform_int_distribution<int> dist(-5, 10);
        uniform_int_distribution<int> miss(0, 4);

        if (miss(gen) == 0) {
            cout << YELLOW << "Attack has missed!" << RESET << endl;
            return;
        }

        int damage = (attack + strengthBonus) - (target.defense + target.defenseBonus) + dist(gen);
        if (damage < 1)
            damage = 1;

        target.health -= damage;
        cout << "Attack did " << RED << damage << RESET << " damage!" << endl;
    }

    virtual void specialMove(Character& target) {
        if (specialCooldown > 0) {
            cout << YELLOW << "Special on cooldown (" << specialCooldown << ")" << RESET << endl;
            return;
        }
        cout << "No special move defined.\n";
    }

    void heal() {
        health += 15;
        cout << GREEN << label << " healed 15 HP!" << RESET << endl;
    }

    void strength() {
        strengthBonus += 5;
        strengthTurns += 4;
        cout << MAGENTA << label << " gained +5 attack for 3 turns!" << RESET << endl;
    }

    void updateEffects() {
        if (strengthTurns > 0) {
            cout << label << ": Strength for " << MAGENTA << strengthTurns - 1 << RESET << " remaining turns!!" << endl;
            strengthTurns--;
            if (strengthTurns == 0) {
                strengthBonus = 0;
                cout << MAGENTA << label << "Strength boost wore off!" << RESET << endl;
            }
        }

        if (specialCooldown > 0)
            specialCooldown--;

        if (defenseTurns > 0)
        {
            cout << label << ": Defense boost for "
                 << BLUE << defenseTurns - 1 << RESET << " remaining turns!!" << endl;

            defenseTurns--;

            if (defenseTurns == 0)
            {
                defenseBonus = 0;
                cout << BLUE << label << " defense boost wore off!" << RESET << endl;
            }
        }
    }

    bool isAlive() {
        return health > 0;
    }

    int getHealth() {
        return health;
    }

    string getType() {
        return type;
    }

    void takeDamage(int dmg) {
        health -= dmg;
    }

    void healAmount(int amount) {
        health += amount;
    }

    int getSpecialCooldown() {
        return specialCooldown;
    }
};

// character types
class Light : public Character {
public:
    Light() : Character("Light", 80, 18, 5) {}

    void specialMove(Character& target) override {
        if (specialCooldown > 0) {
            cout << "Light special on cooldown!\n";
            return;
        }

        int damage = attack * 2;
        target.takeDamage(damage);
        cout << CYAN << label << " used DOUBLE STRIKE for " << damage << " damage!" << RESET << endl;

        specialCooldown = maxCooldown;
    }
};

class Medium : public Character {
public:
    Medium() : Character("Medium", 100, 14, 8) {}

    void specialMove(Character& target) override {
        if (specialCooldown > 0) {
            cout << "Medium special on cooldown!\n";
            return;
        }

        int damage = attack + 5;
        target.takeDamage(damage);
        healAmount(10);
        health += 10;
        cout << GREEN << label << " used LIFE STEAL: " << damage << " damage + heal 10!" << RESET << endl;


        specialCooldown = maxCooldown;
    }
};

class Heavy : public Character {
public:
    Heavy() : Character("Heavy", 125, 11, 12) {}

    void specialMove(Character& target) override {
        if (specialCooldown > 0) {
            cout << "Heavy special on cooldown!\n";
            return;
        }

        defenseBonus += 5;
        defenseTurns = 4;
        cout << BLUE << label << " used FORTIFY: Defense increased!" << RESET << endl;

        specialCooldown = maxCooldown;
    }
};

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
        cout << "1. Light (Inflict High Damage, Low Health || Special Move: DOUBLE STRIKE)" << endl;
        cout << "2. Medium (Balanced Damage and Health || Special Move: LIFE STEAL)" << endl;
        cout << "3. Heavy (Inflict Low Damage, High Health || Special Move: FORTIFY)" << endl;
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
