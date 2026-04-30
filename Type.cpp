#include "Type.h"
#include "Character.h"

// colors for text output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace std;

// constructors
Light::Light() : Character("Light", 80, 18, 5) {}

Medium::Medium() : Character("Medium", 100, 14, 8) {}

Heavy::Heavy() : Character("Heavy", 125, 11, 12) {}


// special moves
void Light::specialMove(Character& target) {
    if (specialCooldown > 0) {
        cout << "Light special on cooldown!\n";
        return;
    }

    int damage = attack * 2;
    target.takeDamage(damage);

    cout << CYAN << label << " used DOUBLE STRIKE for " << damage << " damage!" << RESET << endl;

    specialCooldown = maxCooldown;
}

void Medium::specialMove(Character& target) {
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

void Heavy::specialMove(Character& target) {
    if (specialCooldown > 0) {
        cout << "Heavy special on cooldown!\n";
        return;
    }

    defenseBonus += 5;
    defenseTurns = 4;

    cout << BLUE << label << " used FORTIFY: Defense increased + 5!" << RESET << endl;

    specialCooldown = maxCooldown;
}