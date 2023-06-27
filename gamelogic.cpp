#include "global.h"
#include "gamecontrol.h"


//---------------------------------------
// nucli principal de la lògica del joc
//---------------------------------------
void GameController::playersParty (Player* sourcePlayer, Player* targetPlayer)
{
    // són dos jugadors vàlids
    if (sourcePlayer != nullptr && targetPlayer != nullptr)
    {   // analitzem la 'trobada'
        switch (sourcePlayer->getType())
        {
        case PLAYER_TYPE_SHEEP:
            switch (targetPlayer->getType())
            {
            case PLAYER_TYPE_APPLE: // troba una poma i se la menja
                playersLunch (sourcePlayer, targetPlayer);
                break;
            case PLAYER_TYPE_SHEEP: // troba un altre bé
                if (alNumGen(0,9,1) < targetPlayer->getEmpathy())    // si l'hi cau prou bé... uhm...
                    // no apte per a la canalla ;-)
                    sourcePlayer->playerPair();
                else    // s'esbatussen
                    playersDance (sourcePlayer, targetPlayer);
                break;
            case PLAYER_TYPE_WOLF:  // troba un llop
                if (alNumGen(0,9,1) < targetPlayer->getBravery()) // segons com de valent sigui
                    playersDance (sourcePlayer, targetPlayer);
                else    // fuig
                    targetPlayer->setCompass(sourcePlayer->getCompass());
                break;
            default:
                break;
            }
            break;
        case PLAYER_TYPE_WOLF:
            switch (targetPlayer->getType())
            {
            case PLAYER_TYPE_APPLE: // troba una poma...
                // potser se la menja
                if (sourcePlayer->getEnergy() <= sourcePlayer->getDrainEnergy()
                    && alNumGen(0,1,1)==1) // hi ha gana i l'hi va la vida, però no menja sempre
                    // doncs sí
                    playersLunch (sourcePlayer, targetPlayer);
                break;
            case PLAYER_TYPE_SHEEP:     // troba un bé...
                // potser se'l menja
                if ((alNumGen(0,9,1) < targetPlayer->getBravery()))   // segons com sigui de valent l'altre
                    // doncs sí
                    playersLunch (sourcePlayer, targetPlayer);
                else    // s'esbatussen
                    playersDance(sourcePlayer, targetPlayer);
                break;
            case PLAYER_TYPE_WOLF:  // troba un llop
                // no tots els contactes són bons
                if ((alNumGen(0,9,1) < targetPlayer->getEmpathy()))   // si li cau prou bé... uhm...
                    // no apte per a la canalla ;-)
                    sourcePlayer->playerPair();
                else    // s'esbatussen
                    playersDance(sourcePlayer, targetPlayer);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

//---------------------------------------
// processa les topades entre jugadors
//---------------------------------------

void GameController::playersDance(Player* sourcePlayer, Player* targetPlayer)
{
    // qui topa pren mal
    sourcePlayer->playerDrainEnergy(sourcePlayer->getDrainEnergy());
    // i l'altre a més a més... s'enfila...
    targetPlayer->playerDrainEnergy(targetPlayer->getDrainEnergy()+PLAYER_DRAINED_ONFLEE);
    // cap a l'altra banda
    targetPlayer->setCompass(sourcePlayer->getCompass());
}



//-------------------------------------------
// bé, un dels dos és el plat principal ;-)
//-------------------------------------------

void GameController::playersLunch(Player* playerEating, Player* playerEaten)
{
int eatingEnergyTo=0;
int energyInOut=0;

    if (playerEating!=nullptr && playerEaten!=nullptr)
    {   // energia que hauria de consumir qui menja
        switch (playerEating->getType())
        {
        case PLAYER_TYPE_SHEEP:
            eatingEnergyTo=SHEEP_MAX_ENERGY-playerEating->getEnergy();
            break;
        case PLAYER_TYPE_WOLF:
            eatingEnergyTo=WOLF_MAX_ENERGY-playerEating->getEnergy();
            break;
        default:
            return;
        }
        // calculem l'energia a transsaccionar
        energyInOut=(playerEaten->getEnergy() >= eatingEnergyTo)? eatingEnergyTo : playerEaten->getEnergy();
        playerEating->playerEating(energyInOut);

//        playerEaten->playerEaten(energyInOut);    // consumim tant sols una mica, segueis viu
        playerEaten->playerEating(playerEaten->getEnergy()); // consumim tota la seva energia, es mor
    }
}
