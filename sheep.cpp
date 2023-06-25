#include "global.h"
#include "sheep.h"

Sheep::Sheep()
{
    // característiques i estats
    type=PLAYER_TYPE_SHEEP;
    evolution=0;
    status=PLAYER_STATUS_LIVE;
    empathy=8;
    bravery=1;
    pairs=0;
    active=true;
    clonable=false;
    onPlay=true;

    // energia
    energy=SHEEP_MAX_ENERGY;
    energyDrainOnClone=SHEEP_DRAINED_ENERGY;
    energyDrainOnMove=PLAYER_DRAINED_ONMOVE;

    // clonatge
    cloneOnPairs=SHEEP_CLONE_ON;

    // posicionament
    compass.setSense(COMPASS_E);
};

Sheep* Sheep::Clone()
{
Sheep* newSheep=nullptr;

    // en fem una còpia
    newSheep = new Sheep ();
    // si ha reeixit
    if (newSheep != nullptr)
    {   // noves característiques i estats
        newSheep->evolution=evolution;

        //----------------------------------------------
        // sense variacions genètiques
        //----------------------------------------------
        newSheep->empathy=empathy;
        newSheep->bravery=bravery;
        newSheep->energyDrainOnClone=energyDrainOnClone;
        newSheep->energyDrainOnMove=energyDrainOnMove;
    }
    return newSheep;
}

Sheep* Sheep::Raise()
{
Sheep* newSheep=nullptr;

    // si en tenim prou de forces ;-)
    if (getEnergy() >= SHEEP_DRAINED_ENERGY*2)
    {   // en parim una ;-)
        newSheep = new Sheep ();
        // si ha reeixit
        if (newSheep != nullptr)
        {   // noves característiques i estats
            newSheep->evolution=evolution+1;
            // energia, potser la 'putegem' una mica ;-)
            newSheep->energy=energy;

            playerAction(PLAYER_ACTION_CLONE);

            //----------------------------------------------
            // variacions genètiques
            //----------------------------------------------
            // (alNumGen(0,9,1)<=1) - índex de mutabilitat
            // (alNumGen(0,1,1)==0) - sentit de la mutació
            //----------------------------------------------
            newSheep->empathy=(empathy+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -1 : 1):0));

            if(newSheep->empathy<0)   // mínim 0
                newSheep->empathy=0;
            if(newSheep->empathy>9)   // màxim 9
                newSheep->empathy=0;

            newSheep->bravery=(bravery+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -1 : 1):0));
            if(newSheep->bravery<0)   // mínim 0
                newSheep->bravery=0;
            if(newSheep->bravery>9)   // màxim 9
                newSheep->bravery=0;

            newSheep->energyDrainOnClone=(energyDrainOnClone+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -SHEEP_DRAINED_ENERGY : SHEEP_DRAINED_ENERGY):0));
            if(newSheep->energyDrainOnClone<0)   // mínim 0
                newSheep->energyDrainOnClone=0;

            newSheep->energyDrainOnMove=(energyDrainOnMove+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -PLAYER_DRAINED_ONMOVE : PLAYER_DRAINED_ONMOVE):0));
            if(newSheep->energyDrainOnMove<0)   // mínim 0
                newSheep->energyDrainOnMove=0;
        }
    }
    return newSheep;
}
