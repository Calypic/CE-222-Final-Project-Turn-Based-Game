#pragma once

#include <string>
#include <random>
#include <iostream>

using namespace std;

extern mt19937 gen;

class Character {
protected:
    int health;
    int attack;
    int defense;
    int strengthBonus;
    int strengthTurns;
    int defenseBonus;
    int defenseTurns;
    int specialCooldown;
    int maxCooldown;
    string type;
    string label;

public:
    Character(string t, int h, int a, int d);
    virtual ~Character();

    void setLabel(string l);

    virtual void attackTarget(Character& target);
    virtual void specialMove(Character& target);

    void heal();
    void strength();
    void updateEffects();

    bool isAlive();
    int getHealth();
    string getType();

    void takeDamage(int dmg);
    void healAmount(int amount);
    int getSpecialCooldown();
};