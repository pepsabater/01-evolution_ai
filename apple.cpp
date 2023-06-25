#include "global.h"
#include "apple.h"

Apple::Apple()
{
    // característiques i estats
    type=PLAYER_TYPE_APPLE;
    evolution=0;
    status=PLAYER_STATUS_LIVE;
    active=false;     // és estàtic!
    clonable=false;
    onPlay=true;

    // energia
    energy=APPLE_MAX_ENERGY;
    energyDrainOnClone=APPLE_DRAINED_ENERGY;
    energyDrainOnMove=0;      // és estàtic

    // posicionament
    compass.setSense(NO_COMPASS);  // és estàtic!
};

Apple* Apple::Clone()
{
    return nullptr;
}

Apple* Apple::Raise()
{
    return nullptr;
}
