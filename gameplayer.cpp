#include "global.h"
#include "gamecontrol.h"

//---------------------
// belluga el jugador
//---------------------
bool GameController::playerMove (Player* player, int action)
{
bool retval=false;

    // si és un jugador vàlid
    if (player!=nullptr)
    {   // hi haurà moviment/acció
        retval=true;
        // a veure que ens demanen de fer
        switch (action)
        {
        case PLAYER_ACTION_GO:
            playerGo(player);
            break;
        case PLAYER_ACTION_TLEFT:
            playerGoLeft(player);
            break;
        case PLAYER_ACTION_TRIGHT:
            playerGoRight(player);
            break;
        case PLAYER_ACTION_REVERSE:
            playerGoReverse(player);
            break;
        case PLAYER_ACTION_WAIT:
            playerWait(player);
            break;
        case PLAYER_ACTION_PURSUE:
            playerPursue(player);
            break;
        case PLAYER_ACTION_FLEE:
            playerFlee(player);
            break;
        case PLAYER_ACTION_FIGHTH:
            playerFight(player);
            break;
        case PLAYER_ACTION_PAIR:
            playerPair(player);
            break;
        case PLAYER_ACTION_EATING:
            playerEat(player);
            break;
        default:    // no sap on vol anar!
            retval=false;
        }
    }
    return retval;
}

void GameController::playerGo (Player* player)
{
int newxpos=0;
int newypos=0;

    // si no és un jugador vàlid
    if (player==nullptr)
        return;

    // enregistrem la seva posicio
    newxpos=player->getXPos();
    newypos=player->getYPos();

    switch (player->getCompass())
    {   // recalculem la posició segon la brúixola
    case COMPASS_N:
        newypos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NE:
        newxpos+=PLAYER_XDRIFT;
        newypos-=PLAYER_YDRIFT;
        break;
    case COMPASS_E:
        newxpos+=PLAYER_XDRIFT;
        break;
    case COMPASS_SE:
        newxpos+=PLAYER_XDRIFT;
        newypos+=PLAYER_YDRIFT;
        break;
    case COMPASS_S:
        newypos+=PLAYER_YDRIFT;
        break;
    case COMPASS_SW:
        newxpos-=PLAYER_XDRIFT;
        newypos+=PLAYER_YDRIFT;
        break;
    case COMPASS_W:
        newxpos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NW:
        newxpos-=PLAYER_XDRIFT;
        newypos-=PLAYER_YDRIFT;
        break;
    default:    // no sap on vol anar!
        return;
    }

    // si la nova posició és fora d'escena...
    if ((newxpos < 0 || newxpos > VIEW_WIDTH-PIXMAP_WIDTH)
        || (newypos < 0 || newypos > VIEW_HEIGHT-PIXMAP_HEIGHT))
        // fem màgia!
        switch (player->getCompass())
        {
        case COMPASS_N:
            newypos = VIEW_HEIGHT - PIXMAP_HEIGHT;
            break;
        case COMPASS_NE:
            if (newxpos + PIXMAP_WIDTH >= VIEW_WIDTH - PIXMAP_WIDTH)
                newxpos = 0;

            if (newypos - PIXMAP_HEIGHT <= 0 )
                newypos = VIEW_HEIGHT - PIXMAP_HEIGHT;
            break;
        case COMPASS_E:
            newxpos = 0;
            break;
        case COMPASS_SE:
            if (newxpos + PIXMAP_WIDTH >= VIEW_WIDTH - PIXMAP_WIDTH)
                newxpos = 0;

            if (newypos + PIXMAP_HEIGHT >= VIEW_HEIGHT - PIXMAP_HEIGHT)
                newypos = 0;
            break;
        case COMPASS_S:
            newypos = 0;
            break;
        case COMPASS_SW:
            if (newxpos - PIXMAP_WIDTH <= 0)
                newxpos = VIEW_WIDTH - PIXMAP_WIDTH;

            if (newypos + PIXMAP_HEIGHT >= VIEW_HEIGHT - PIXMAP_HEIGHT)
                newypos = 0;
            break;
        case COMPASS_W:
            newxpos = VIEW_WIDTH - PIXMAP_WIDTH;
            break;

        case COMPASS_NW:
            if (newxpos - PIXMAP_WIDTH <= 0)
                newxpos = VIEW_WIDTH - PIXMAP_WIDTH;

            if (newypos - PIXMAP_HEIGHT <= 0)
                newypos = VIEW_HEIGHT - PIXMAP_HEIGHT;
            break;
        }
    player->setPos(newxpos, newypos);
}

void GameController::playerWait(Player* player)
{}

void GameController::playerPursue(Player* player)
{}

void GameController::playerFlee(Player* player)
{}

void GameController::playerFight(Player* player)
{}

void GameController::playerPair(Player* player)
{}

void GameController::playerEat(Player* player)
{}

//--------------------------------
// el jugador tomba a l'esquerra
//--------------------------------

void GameController::playerGoLeft(Player* player)
{
int leftDir;

    if (player!=nullptr)
    {   // un tomb a l'esquerra tanca l'angle
        leftDir=player->getCompass() - 1;
        (leftDir < 0)? player->setCompass(COMPASS_STEPS - 1) : player->setCompass(leftDir);
        player->pointTo();
        playerRotation(player);
    }
}

//----------------------
// el jugador gira cua
//----------------------

void GameController::playerGoReverse(Player* player)
{
int newDir;

    if (player!=nullptr)
    {
        newDir=player->getCompass() + COMPASS_STEPS/2;
        (newDir >= COMPASS_STEPS)? player->setCompass(newDir-COMPASS_STEPS) : player->setCompass(newDir);
        player->pointTo();
        playerRotation(player);
    }
}

//-------------------------------
// el jugador tomba a la dreta
//-------------------------------

void GameController::playerGoRight(Player* player)
{
int rightDir;

    if (player!=nullptr)
    {   // un tomb a la dreta obre l'angle
        rightDir=player->getCompass() + 1;
        (rightDir >= COMPASS_STEPS)? player->setCompass(0) : player->setCompass(rightDir);
        player->pointTo();
        playerRotation(player);
    }
}

//-------------------------------------
// compta jugadors per mena i l'estat
//-------------------------------------
// fa servir el vector de jugadors

int GameController::playerCounter(int playerType, int playerStatus)
{
int counter;
int players=0;

    for (counter=0; (unsigned long)counter < playersPool.size(); counter++)
        if (playersPool[counter]->getType()==playerType
        && playersPool[counter]->getStatus()==playerStatus
        && playersPool[counter]->isOnPlay()==true)
            players++;
    return players;
}

//-----------------------------------------
// retorna el jugador que hi ha a un lloc
//-----------------------------------------
// fa servir el vector de jugadors

Player* GameController :: playerWhoIs(int xpos, int ypos)
{
int counter;
Player* foundPlayer = nullptr;

    for(counter=0; (unsigned long)counter < playersPool.size(); counter++)
    {
        foundPlayer=playersPool[counter];
        if (foundPlayer->getXPos() == xpos
            && foundPlayer->getYPos() == ypos
            && foundPlayer->isOnPlay()==true)
            break;
        else
            foundPlayer=nullptr;
    }
    return foundPlayer;
}

int GameController::isPlayer(QGraphicsItem* graphitem)
{   // és jugador o una altre cosa?
int retval=PLAYER_NONE;
int counter;

    if (graphitem== nullptr)
        return retval;

    for(counter=0; (unsigned int)counter < playersPool.size(); counter++)
        if(playersPool[counter]->getPixmap()==graphitem)
        {
            retval=counter;
            break;
        }
    return retval;
}

bool GameController::playerCollision(Player* player)
{   // es validen els contactes del jugador
QList<QGraphicsItem*> playerslist;
int playeritems=0;
bool retval;

    if (player==nullptr)
        return false;

    // si té cercle de captura té un item+
    if (player->getVisionItem()!=nullptr)
        playeritems++;

    playerslist=gameScene->collidingItems(player->getPixmap());
    retval=(playerslist.size() > playeritems)? true : false;
    return retval;
}

int GameController::playerOnFocus(Player* player)
{   // es validen els contactes del jugador 0
int counter;
int loadplayer=PLAYER_NONE;
int foundplayer=PLAYER_NONE;
QList<QGraphicsItem*> playerslist;

    if (player==nullptr || player->isActive() == false)
        return foundplayer;

    playerslist=gameScene->collidingItems(player->getVisionItem());

    for (counter=0; counter < playerslist.size(); counter++)
    {
        loadplayer=isPlayer(playerslist[counter]);
        if (loadplayer != PLAYER_NONE && playersPool[loadplayer] != player)
        {
            foundplayer=loadplayer;
            break;
        }
    }
    return foundplayer;
}

int GameController::playerOnContact(Player* player)
{   // es validen els contactes del jugador 0
int counter;
int loadplayer=PLAYER_NONE;
int foundplayer=PLAYER_NONE;
QList<QGraphicsItem*> playerslist;

    if (player==nullptr || player->isActive() == false)
        return foundplayer;

    playerslist=gameScene->collidingItems(player->getPixmap());

    for (counter=0; counter < playerslist.size(); counter++)
    {
        loadplayer=isPlayer(playerslist[counter]);
        if (loadplayer != PLAYER_NONE && playersPool[loadplayer] != player)
        {
            foundplayer = loadplayer;
            break;
        }
    }
    return foundplayer;
}
