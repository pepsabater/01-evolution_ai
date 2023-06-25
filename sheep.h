#ifndef SHEEP_H
#define SHEEP_H

#include "player.h"

class Sheep : public Player
{
public:
    Sheep();
    Sheep* Clone();
    Sheep* Raise();
};

#endif // SHEEP_H
