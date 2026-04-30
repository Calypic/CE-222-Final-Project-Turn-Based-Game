#pragma once

#include "Character.h"

class Light : public Character {
public:
    Light();
    void specialMove(Character& target) override;
};

class Medium : public Character {
public:
    Medium();
    void specialMove(Character& target) override;
};

class Heavy : public Character {
public:
    Heavy();
    void specialMove(Character& target) override;
};