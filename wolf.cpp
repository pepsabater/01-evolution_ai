#include "global.h"

#include "wolf.h"

Wolf::Wolf()
{
    // característiques i estats
    type=PLAYER_TYPE_WOLF;
    evolution=0;
    status=PLAYER_STATUS_LIVE;
    empathy=8;
    bravery=1;
    pairs=0;
    active=true;
    clonable=false;
    onPlay=true;

    // energia
    energy=WOLF_MAX_ENERGY;
    energyDrainOnClone=WOLF_DRAINED_ENERGY;
    energyDrainOnMove=PLAYER_DRAINED_ONMOVE;

    // clonatge
    cloneOnPairs=WOLF_CLONE_ON;

    // posicionament
    compass.setSense(COMPASS_W);
};

Wolf* Wolf::Clone()
{
Wolf* newWolf=nullptr;

    // en fem una còpia
    newWolf = new Wolf ();
    // si ha reeixit
    if (newWolf != nullptr)
    {   // noves característiques i estats
        newWolf->evolution=evolution;

        playerAction(PLAYER_ACTION_CLONE);

        //----------------------------------------------
        // sense variacions genètiques
        //----------------------------------------------
        newWolf->empathy=empathy;
        newWolf->bravery=bravery;
        newWolf->energyDrainOnClone=energyDrainOnClone;
        newWolf->energyDrainOnMove=energyDrainOnMove;
    }
    return newWolf;
}

Wolf* Wolf::Raise()
{
Wolf* newWolf=nullptr;

    // si en tenim prou de forces ;-)
    if (energy >= WOLF_DRAINED_ENERGY*2)
    {
        // en parim un ;-)
        newWolf = new Wolf ();
        // si ha reeixit
        if (newWolf != nullptr)
        {   // noves característiques i estats
            newWolf->evolution=evolution+1;
            // energia, potser la 'putegem' una mica ;-)
            newWolf->energy=energy;
            playerAction(PLAYER_ACTION_CLONE);

            //----------------------------------------------
            // variacions genètiques
            //----------------------------------------------
            // (alNumGen(0,9,1)<=1) - índex de mutabilitat
            // (alNumGen(0,1,1)==0) - sentit de la mutació
            //----------------------------------------------
            newWolf->empathy=(empathy+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -1 : 1):0));

            if(newWolf->empathy<0)   // mínim 0
                newWolf->empathy=0;
            if(newWolf->empathy>9)   // màxim 9
                newWolf->empathy=0;

            newWolf->bravery=(bravery+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -1 : 1):0));
            if(newWolf->bravery<0)   // mínim 0
                newWolf->bravery=0;
            if(newWolf->bravery>9)   // màxim 9
                newWolf->bravery=0;

            newWolf->energyDrainOnClone=(energyDrainOnClone+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -WOLF_DRAINED_ENERGY : WOLF_DRAINED_ENERGY):0));
            if(newWolf->energyDrainOnClone<0)   // mínim 0
                newWolf->energyDrainOnClone=0;

            newWolf->energyDrainOnMove=(energyDrainOnMove+((alNumGen(0,9,1)==0)?((alNumGen(0,1,1)==0)? -PLAYER_DRAINED_ONMOVE : PLAYER_DRAINED_ONMOVE):0));
            if(newWolf->energyDrainOnMove<0)   // mínim 0
                newWolf->energyDrainOnMove=0;
        }
    }
    return newWolf;
}
