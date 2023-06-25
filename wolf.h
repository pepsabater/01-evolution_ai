#ifndef WOLF_H
#define WOLF_H

#include "player.h"

class Wolf : public Player
{
public:
    Wolf();
    Wolf* Clone();
    Wolf* Raise();
};

#endif // WOLF_H
