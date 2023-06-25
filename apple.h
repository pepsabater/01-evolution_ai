#ifndef APPLE_H
#define APPLE_H

#include "player.h"

class Apple: public Player
{

public:
    Apple();
    Apple* Clone();
    Apple* Raise();
};

#endif // APPLE_H
