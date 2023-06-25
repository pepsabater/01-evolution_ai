#include "player.h"

Player::Player()
{
    // característiques i estats
    type=0;
    evolution=0;
    status=0;
    empathy=0;
    bravery=0;
    pairs=0;
    active=false;
    clonable=false;
    onPlay=false;

    // energia
    energyDrainOnClone=0;
    energyDrainOnMove=0;
    energyToLost=0;
    energyToGain=0;

    // clonatge
    cloneOnPairs=0;  // per ara no por clonar-se

    // posicionament i orientació
    compass.setPos(0, 0);
    compass.setSense(COMPASS_N);

    // inicialització de les estadístiques
    stStepsTaken=0;      // passes fetes
    stEnergyWin=0;       // energia obtinguda
    stEnergyLost=0;      // energia gastada
    stWaitsTaken=0;      // cops que s'ha aturat
    stTimesCloned=0;     // cops que ha clonat
    stTotalPairs=0;   // interaccions totals
};

bool Player::updateAction(int action)
{
int compassSense=compass.getSense();

    switch(action)
    {
    case PLAYER_ACTION_WAIT:
        energyToLost=energyDrainOnMove;
        stWaitsTaken++;
        break;
    case PLAYER_ACTION_WALK:
        energyToLost=energyDrainOnMove;
        stStepsTaken++;
        break;
    case PLAYER_ACTION_LEFT:
        compass.setSense((compassSense - 1 < 0)? COMPASS_NW : compassSense - 1);
        energyToLost=energyDrainOnMove;
        stStepsTaken++;
        break;
    case PLAYER_ACTION_RIGHT:
        compass.setSense((compassSense + 1 == COMPASS_STEPS)? COMPASS_N : compassSense + 1);
        energyToLost=energyDrainOnMove;
        stStepsTaken++;
        break;
    case PLAYER_ACTION_EATING:
    case PLAYER_ACTION_EATEN:
        break;
    case PLAYER_ACTION_FLEE:
        energyToLost=energyDrainOnMove*3;
        stStepsTaken++;
        break;
    case PLAYER_ACTION_PURSUE:
        energyToLost=energyDrainOnMove*3;
        stStepsTaken++;
        break;
    case PLAYER_ACTION_CLONE:
        energyToLost=energyDrainOnClone;
        pairs=0;
        clonable=false;
        stStepsTaken++;
        break;
    default:
        return false;
    }
    updateEnergy();
    return true;
}

void Player::updateEnergy()
{
    // actualitzar energia i estadístiques
    energy+=energyToGain;
    stEnergyWin+=energyToGain;
    // actualitzar les estadístiques
    energy-=energyToLost;
    stEnergyLost+=energyToLost;
    // actualitzem els estats en base a l'energia
    if (energy<=0)
    {
        energy=0;
        status=PLAYER_STATUS_DEAD;
        active=clonable=false;
    }
    // reiniciem els paràmetres d'energia
    energyToGain=energyToLost=0;
}

void Player::playerPair()
{
    if (cloneOnPairs != 0                // si pot clonar-se
        && pairs+1 >= cloneOnPairs)   // i ha superat el llindar de contactes
    {   // passa a ser clonable
        clonable=true;
        // reiniciem el comptador de contactes
        pairs=0;
    }
    else
        pairs++;
    // actualitem les estadístiques
    stTotalPairs++;
}


