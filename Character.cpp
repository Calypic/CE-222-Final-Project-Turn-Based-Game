#include "Character.h"
#include <string>
#include <random>
#include <iostream>

// colors for text output
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

using namespace std;

// single RNG definition (only here)
mt19937 gen(random_device{}());

Character::Character(string t, int h, int a, int d)
    : type(t), health(h), attack(a), defense(d),
      strengthBonus(0), strengthTurns(0),
      defenseBonus(0), defenseTurns(0),
      specialCooldown(0), maxCooldown(6),
      label("Player") {}

Character::~Character() {}

void Character::setLabel(string l) {
    label = l;
}

void Character::attackTarget(Character& target) {
    uniform_int_distribution<int> dist(-5, 10);
    uniform_int_distribution<int> miss(0, 4);

    if (miss(gen) == 0) {
        cout << YELLOW << "Attack has missed!" << RESET << endl;
        return;
    }

    int damage = (attack + strengthBonus) - (target.defense + target.defenseBonus) + dist(gen);
    if (damage < 1) damage = 1;

    target.health -= damage;
    cout << "Attack did " << RED << damage << RESET << " damage!" << endl;
}

void Character::specialMove(Character& target) {
    if (specialCooldown > 0) {
        cout << YELLOW << "Special on cooldown (" << specialCooldown << ")" << RESET << endl;
        return;
    }
    cout << "No special move defined.\n";
}

void Character::heal() {
    health += 15;
    cout << GREEN << label << " healed 15 HP!" << RESET << endl;
}

void Character::strength() {
    strengthBonus += 5;
    strengthTurns += 4;
    cout << MAGENTA << label << " gained +5 attack for 3 turns!" << RESET << endl;
}

void Character::updateEffects() {
    if (strengthTurns > 0) {
        cout << label << ": Strength for " << MAGENTA << strengthTurns - 1 << RESET << " remaining turns!!" << endl;
        strengthTurns--;
        if (strengthTurns == 0) {
            strengthBonus = 0;
            cout << MAGENTA << label << " Strength boost wore off!" << RESET << endl;
        }
    }

    if (specialCooldown > 0)
        specialCooldown--;

    if (defenseTurns > 0) {
        cout << label << ": Defense boost for " << BLUE << defenseTurns - 1 << RESET << " remaining turns!!" << endl;
        defenseTurns--;

        if (defenseTurns == 0) {
            defenseBonus = 0;
            cout << BLUE << label << " defense boost wore off!" << RESET << endl;
        }
    }
}

bool Character::isAlive() {
    return health > 0;
}

int Character::getHealth() {
    return health;
}

string Character::getType() {
    return type;
}

void Character::takeDamage(int dmg) {
    health -= dmg;
}

void Character::healAmount(int amount) {
    health += amount;
}

int Character::getSpecialCooldown() {
    return specialCooldown;
}