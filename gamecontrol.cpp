#include "global.h"
#include "gamecontrol.h"

GameController::GameController(QGraphicsScene* scene, GameView* view, MyStatsDialog* statsDialog)
{   // assignem l'escena i la vista heretades
    gameScene=scene;
    gameView=view;
    gameStatsDialog=statsDialog;

    // connectem els events de teclat i un temporitzador a la classe
    connect(view, SIGNAL(keyPressEvent(QKeyEvent*)), this, SLOT(touchMe(QKeyEvent*)));
    gameTamTam = new QTimer();
    connect(gameTamTam, SIGNAL(timeout()), this, SLOT(onTamTam()));
}

void GameController::gSetInit ()
{
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setScene(gameScene);
    // fixa les mides i bloca els marcs
    gameView->setFixedSize(VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setFrameStyle(0);
    // defineix la mida de l'escena
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);
    // programa el temporitzador
    gameTamTam->setInterval(GAME_WORLD_TIMER_TIME);
}

void GameController::gSetRestart()
{
    // estadístiques
    if(stLiveSheeps==0)
    {
        stLiveSheeps=0;
        stInSheeps=0;
        stClonedSheeps=0;
        stOutSheeps=0;
        stSheepEvolution=0;
    }

    if(stLiveWolves==0)
    {
        stLiveWolves=0;
        stInWolves=0;
        stClonedWolves=0;
        stOutWolves=0;
        stWolfEvolution=0;
    }
}

void GameController::gSetTheatre ()
{
    // crea un QPixmap amb la imatge de fons
    gameBackImage=new QPixmap(backPixmapPath);
    // crea un pincell utilitzant el QPixmap com a textura
    gameBrush=new QBrush(*gameBackImage);
    // estableix el pincell (brush) com a fons de l'escena
    gameView->setBackgroundBrush(*gameBrush);

    // afegim els jugadors
    gMakePlayers(PLAYER_TYPE_APPLE, INITIAL_APPLES);
    gMakePlayers(PLAYER_TYPE_SHEEP, INITIAL_SHEEPS);
    gMakePlayers(PLAYER_TYPE_WOLF, INITIAL_WOLVES);
}

void GameController::gMakePlayers(int playertype, int numplayers)
{
QString facepixpath;
QGraphicsPixmapItem* facepixmap;
int counter;
Player* newplayer=nullptr;

    switch (playertype)
    {
    case PLAYER_TYPE_APPLE:
        for (counter = 0; counter < numplayers; counter++)
        {   // menjar
            newplayer=new Apple();
            facepixpath=applePixmapPath;
            newplayer->setPixmapPath(facepixpath);
            facepixmap = new QGraphicsPixmapItem(QPixmap(newplayer->getPixmapPath()));
            newplayer->setPixmap(facepixmap);

            if (playerAdd (newplayer)== true)
            {   // estadístiques
                stLiveApples++;
                stInApples++;
                stInAppleTotals++;
            }
        }
        break;
    case PLAYER_TYPE_SHEEP:
        for (counter = 0; counter < numplayers; counter++)
        {   // bens, generats de cero o a partir del que ha trigat més en morir
            if (lastSheep!=nullptr)
                newplayer=lastSheep->Clone();
            else
                newplayer=new Sheep();

            facepixpath=(newplayer->getEvolution()>=SHEEP_PIXMAPS)? sheepPixmapPaths[newplayer->getEvolution()%SHEEP_PIXMAPS]
                : sheepPixmapPaths[newplayer->getEvolution()];

            newplayer->setPixmapPath(facepixpath);
            facepixmap = new QGraphicsPixmapItem(QPixmap(newplayer->getPixmapPath()));
            newplayer->setPixmap(facepixmap);
            newplayer->setVision();

            if (playerAdd (newplayer)==true)
            {   // estadístiques
                stLiveSheeps++;
                stInSheeps++;
                stInSheepTotals++;

                if(newplayer->getEvolution()>stSheepEvolution)
                    stSheepEvolution=newplayer->getEvolution();
            }
        }
        break;
    case PLAYER_TYPE_WOLF:
        for (counter = 0; counter < numplayers; counter++)
        {   // llops, generats de cero o a partir del que ha trigat més en morir
            if (lastWolf!=nullptr)
                newplayer=lastWolf->Clone();
            else
                newplayer=new Wolf();

            facepixpath=(newplayer->getEvolution()>=WOLF_PIXMAPS)? wolfPixmapPaths[newplayer->getEvolution()%WOLF_PIXMAPS]
                : wolfPixmapPaths[newplayer->getEvolution()];

            newplayer->setPixmapPath(facepixpath);
            facepixmap = new QGraphicsPixmapItem(QPixmap(newplayer->getPixmapPath()));
            newplayer->setPixmap(facepixmap);
            newplayer->setVision();

            if (playerAdd (newplayer)==true)
            {   // estadístiques
                stLiveWolves++;
                stInWolves++;
                stInWolfTotals++;

                if(newplayer->getEvolution()>stWolfEvolution)
                    stWolfEvolution=newplayer->getEvolution();
            }
        }
        break;
    default:
        return;
    }
}

//------------------------------------------
// posa a l' dia'hora els elements del joc
//------------------------------------------
void GameController::onTamTam()
{
int counter;
static int growCounter=0;
static int lifeCounter=0;
Player* currPlayer=nullptr;
Player* newPlayer=nullptr;
int newApples=0;

    // posem a l'hora els comptadors de cicles
    growCounter++;
    lifeCounter++;
    // cada 20 passes del rellotge (5 segons)
    if (growCounter == 20)
    {   // comptador de passes a zero
        growCounter = 0;
        //---------------------
        // regenerador de pomes
        //---------------------
        // per cada 4 pomes 'vives' n'afegim una altra
        // si no n'hi ha cap regenerem un quart de màxmin premés
        if (stLiveApples==0)
            newApples = MAX_APPLES / 4;
        else
        {
            if (stLiveApples + (int)(stLiveApples / 4) <= MAX_APPLES)
                newApples = (int)(stLiveApples / 4);
            else
                newApples = MAX_APPLES - stLiveApples;
        }

        gMakePlayers(PLAYER_TYPE_APPLE, newApples);
    }

    // cada 10 passes del rellotge
    if (lifeCounter == 10)
    {   // comptador de passes a 0
        lifeCounter=0;

        //------------------------
        // regenerador de 'vida'
        //------------------------

        // els xais i llops
        for(counter=0; (unsigned long)counter<playersPool.size(); counter++)
        {   // capturem el jugador
            currPlayer=playersPool[counter];
            // si està en condicions el fem criar ;-)
            if (currPlayer->isClonable() == true)
            {   // mirem de fer-ne un
                if (currPlayer->getType()==PLAYER_TYPE_SHEEP && stLiveSheeps < MAX_SHEEPS)
                {
                   newPlayer=currPlayer->Raise();

                    if (newPlayer!=nullptr && playerAdd(newPlayer)==true)
                    {   // estadístiques
                        stLiveSheeps++;
                        stInSheeps++;
                        stInSheepTotals++;
                        stClonedSheeps++;
                        stClonedSheepTotals++;

                        if (newPlayer->getEvolution()>stSheepEvolution)
                            stSheepEvolution=newPlayer->getEvolution();

                        if (stSheepEvolution>stMaxSheepEvolution)
                            stMaxSheepEvolution=stSheepEvolution;
                    }
                }

                if (currPlayer->getType()==PLAYER_TYPE_WOLF && stLiveWolves < MAX_WOLVES)
                {
                    newPlayer=currPlayer->Raise();

                    if (newPlayer!=nullptr && playerAdd (newPlayer)==true)
                    {   // estadístiques
                        stLiveWolves++;
                        stInWolves++;
                        stInWolfTotals++;
                        stClonedWolves++;
                        stClonedWolfTotals++;

                        if (newPlayer->getEvolution()>stWolfEvolution)
                            stWolfEvolution=newPlayer->getEvolution();

                        if (stWolfEvolution>stMaxWolfEvolution)
                            stMaxWolfEvolution=stWolfEvolution;
                    }
                }
            }
        }
    }

    // belluguem els jugadors
    playersMotion();
    // analisi de topades
    sceneCollisions();
    // visualitzar estadístiques
    gGameStatistics();
    gPlayersStatistics();

    // si ha mort tota una espècie!
    if (stLiveSheeps==0 || stLiveWolves==0)
    { // aturem el món!
        gameTamTam->stop();
        // guanyador del joc
        if(stLiveSheeps==0||stLiveWolves==0)
        {
            stIteration++;
            stLastSurvivor=(stLiveSheeps==0)? PLAYER_TYPE_WOLF : PLAYER_TYPE_SHEEP;
            gSetRestart();
            (stLiveSheeps==0)? gMakePlayers(PLAYER_TYPE_SHEEP, INITIAL_SHEEPS)
                             : gMakePlayers(PLAYER_TYPE_WOLF, INITIAL_WOLVES);
        }
    }
}

void GameController::playersMotion()
{
int counter;

    for (counter=0; (unsigned long)counter < playersPool.size(); counter++)
       // movem els jugadors
        if (playersPool[counter]->isActive() == true)
        {
            if (playersPool[counter]->isHunting() == true
                || playersPool[counter]->isFleeing() == true)
            {
                playerMove(playersPool[counter], PLAYER_ACTION_GO);
                playersPool[counter]->isFleeing(false);
            }
            else // forcem un moviment aleatori, per ara
                playerMove(playersPool[counter], ((alNumGen(0,5,1) < 5)? PLAYER_ACTION_GO : alNumGen(0, 2, 1)));
        }
}

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

    // si la nova posició és dins l'escena
    if ((newxpos>=0 && newxpos <= VIEW_WIDTH-PIXMAP_WIDTH)
        && (newypos>=0 && newypos <= VIEW_HEIGHT-PIXMAP_HEIGHT))
        // posem al'hora les coordinades del jugador
        player->setPos(newxpos, newypos);
    else    // si no, retruc
        switch (player->getCompass())
        {
        case COMPASS_N:
        case COMPASS_E:
        case COMPASS_S:
        case COMPASS_W:
            playerGoReverse(player);
            break;
        case COMPASS_NE:
            if (newxpos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_SE:
            if (newxpos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        case COMPASS_SW:
            if (newxpos < 0)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_NW:
            if (newxpos < 0)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        }
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

//----------------------------------------
// afegueix el jugador i l'hi busca lloc
//----------------------------------------
// fa servir el vector de jugadors
bool GameController::playerAdd(Player* newplayer)
{
bool retval=false;

    if (newplayer!=nullptr)
    {   // afegim el jugador a la llista de jugadors
        playersPool.push_back(newplayer);
        // per raons 'estètiques' afegim primer el 'camp de visió'
        if(newplayer->getVisionItem()!=nullptr)
            gameScene->addItem(newplayer->getVisionItem());
        // i ara el jugador
        gameScene->addItem(newplayer->getPixmap());
        // forcem una orientació aleatòria
        if (newplayer->isActive() == true)
        {
            newplayer->setCompass(alNumGen(0, COMPASS_STEPS, 1));
            newplayer->pointTo();
            playerRotation(newplayer);
        }

        do
        {   // el busquem un lloc a l'escena, repetim fins trobar-ne un de lliure
            newplayer->setPos(alNumGen(0, VIEW_WIDTH-PIXMAP_WIDTH, PLAYER_XDRIFT), alNumGen(0, VIEW_HEIGHT-PIXMAP_HEIGHT, PLAYER_YDRIFT));
            // mirem si el lloc és ple
            // si no n'ha trobat cap, sortim
        } while (playerCollision(newplayer) == true);
        retval=true;
    }
    return retval;
}

//-----------------------------------------------
// control del joc per teclat (per proves)
//-----------------------------------------------

void GameController::touchMe (QKeyEvent* keyEvent)
{
static Player* gamePlayer0=nullptr;
int playeraction=0;

    if (gamePlayer0 == nullptr)
    {
        gamePlayer0=playerFinder(PLAYER_TYPE_SHEEP);
        if (gamePlayer0==nullptr)
            return;
    }
    switch (keyEvent->key())
    {
    case Qt::Key_Up:
        playeraction=PLAYER_ACTION_GO;
        break;
    case Qt::Key_Left:
        playeraction=PLAYER_ACTION_TLEFT;
        break;
    case Qt::Key_Down:
        playeraction=PLAYER_ACTION_REVERSE;
        break;
    case Qt::Key_Right:
        playeraction=PLAYER_ACTION_TRIGHT;
        break;
    case Qt::Key_Space:
        // aturar/endegar el rellotge
        (gameTamTam->isActive()==true)? gameTamTam->stop() : gameTamTam->start();
        return;
    default:
        return;
    }
    playerMove(gamePlayer0, playeraction);
}

//-----------------------------------------------
// dibuixem el jugador amb la nova orientació
//-----------------------------------------------
// si rotation = true, és que cal girar-lo
// si rotation = false, és un desplaçament lineal
//-----------------------------------------------

void GameController::playerRotation(Player *player)
{
QGraphicsPixmapItem* pixmap;
QGraphicsEllipseItem* visionitem;

    if (player==nullptr || player->getPixmap() == nullptr)
        return;

    pixmap=player->getPixmap();
    // fixem el punt de gir fem la rotació
    pixmap->setTransformOriginPoint(pixmap->boundingRect().center());
    pixmap->setRotation(player->getCompassAngle());

    visionitem=player->getVisionItem();

    if(visionitem!=nullptr)
    {   // fixem el punt de gir fem la rotació
        visionitem->setTransformOriginPoint(visionitem->rect().center());
        visionitem->setRotation(player->getCompassAngle());
    }
}

Player* GameController::playerFinder(int playerType)
{
int counter;
Player* foundPlayer=nullptr;

    for (counter=0; (unsigned long)counter<playersPool.size(); counter++)
        if (playersPool[counter]->getType()==playerType)
        {
            foundPlayer = playersPool[counter];
            break;
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

void GameController::sceneCollisions()
{   // es validen els contactes del jugador 0
int oncontactplayer=PLAYER_NONE;
int onfocusplayer=PLAYER_NONE;
int counter=0;

    for (counter = 0; (unsigned)counter < playersPool.size(); counter++)
    {
        oncontactplayer=playerOnContact(playersPool[counter]);

        if (oncontactplayer != PLAYER_NONE && playersPool[oncontactplayer]->isOnPlay() == true
            && (playersPool[counter]->getType() - playersPool[oncontactplayer]->getType()) == 1)
        {   // si n'hi ha cap en contacte, ens el pelem! - per ara
            playersPool[counter]->isHunting(false);

            switch (playersPool[oncontactplayer]->getType())
            {   // estadístiques
            case PLAYER_TYPE_APPLE:
                stLiveApples--;
                stOutApples++;
                stOutAppleTotals++;
                break;
            case PLAYER_TYPE_SHEEP:
                lastSheep=playersPool[oncontactplayer];
                stLiveSheeps--;
                stOutSheeps++;
                stOutSheepTotals++;
                break;
            case PLAYER_TYPE_WOLF:
                lastWolf=playersPool[oncontactplayer];
                stLiveWolves--;
                stOutWolves++;
                stOutWolfTotals++;
                break;
            }
            if (playersPool[oncontactplayer]->getVisionItem() != nullptr)
                gameScene->removeItem(playersPool[oncontactplayer]->getVisionItem());
            gameScene->removeItem(playersPool[oncontactplayer]->getPixmap());
            playersPool[oncontactplayer]->playerPlayOff();
        }
        else
        {
            onfocusplayer=playerOnFocus(playersPool[counter]);

            if (onfocusplayer != PLAYER_NONE && playersPool[onfocusplayer]->isOnPlay() == true)
                switch (playersPool[counter]->getType() - playersPool[onfocusplayer]->getType())
                {
                case -1:    // és un depredador, fugim
                    playersPool[counter]->isFleeing(true);
                    playerGoReverse(playersPool[counter]);
                    break;
                case 0: // és un 'igual'
                    playersPool[counter]->isHunting(false);
                    playersPool[counter]->isFleeing(false);
                    break;
                case 1: // és menjar, el cerquem
                    playersPool[counter]->isHunting(true);
                    playersPool[counter]->isFleeing(false);
                    playersPool[counter]->pointTo(playersPool[onfocusplayer]->getXPos(), playersPool[onfocusplayer]->getYPos());
                    playerRotation(playersPool[counter]);
                    break;
                }

        }
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

//-----------------------------------------------------------
// posar a l'hora les estadístiques i mostrar-les al diàleg
//-----------------------------------------------------------

void GameController::gGameStatistics()
{   // generals del joc
    gameStatsDialog->stGameBoxDataValues->clear();
    gameStatsDialog->stGameBoxDataValues->append(QString::number(stIteration));
    gameStatsDialog->stGameBoxDataValues->append(QString::number(stLiveApples+stLiveSheeps+stLiveWolves));
    gameStatsDialog->stGameBoxDataValues->append(QString::number(stInApples+stInSheeps+stInWolves));
    gameStatsDialog->stGameBoxDataValues->append(QString::number(stClonedSheeps+stClonedWolves));
    gameStatsDialog->stGameBoxDataValues->append(QString::number(stOutApples+stOutSheeps+stOutWolves));


    gameStatsDialog->stGameBoxDataTotals->clear();
    gameStatsDialog->stGameBoxDataTotals->append("game");
    gameStatsDialog->stGameBoxDataTotals->append("totals");
    gameStatsDialog->stGameBoxDataTotals->append(QString::number(stInAppleTotals+stInSheepTotals+stInWolfTotals));
    gameStatsDialog->stGameBoxDataTotals->append(QString::number(stClonedSheepTotals+stClonedWolfTotals));
    gameStatsDialog->stGameBoxDataTotals->append(QString::number(stOutAppleTotals+stOutSheepTotals+stOutWolfTotals));
    gameStatsDialog->stGameBoxDataTotals->append(survivorPlayerName[stLastSurvivor]);
}

void GameController::gPlayersStatistics()
{    // de les pomes
    gameStatsDialog->stApplesBoxDataValues->clear();
    gameStatsDialog->stApplesBoxDataValues->append(QString::number(stLiveApples));
    gameStatsDialog->stApplesBoxDataValues->append(QString::number(stInApples));
    gameStatsDialog->stApplesBoxDataValues->append(QString::number(stOutApples));
    gameStatsDialog->stSheepsBoxDataValues->clear();
    // totals de l'iteració actual
    gameStatsDialog->stApplesBoxDataTotals->clear();
    gameStatsDialog->stApplesBoxDataTotals->append("totals");
    gameStatsDialog->stApplesBoxDataTotals->append(QString::number(stInAppleTotals));
    gameStatsDialog->stApplesBoxDataTotals->append(QString::number(stOutAppleTotals));

    // dels xais
    gameStatsDialog->stSheepsBoxDataValues->clear();
    gameStatsDialog->stSheepsBoxDataValues->append(QString::number(stLiveSheeps));
    gameStatsDialog->stSheepsBoxDataValues->append(QString::number(stInSheeps));
    gameStatsDialog->stSheepsBoxDataValues->append(QString::number(stClonedSheeps));
    gameStatsDialog->stSheepsBoxDataValues->append(QString::number(stOutSheeps));
    gameStatsDialog->stSheepsBoxDataValues->append(QString::number(stSheepEvolution));
    // totals de l'iteració actual
    gameStatsDialog->stSheepsBoxDataTotals->clear();
    gameStatsDialog->stSheepsBoxDataTotals->append("totals");
    gameStatsDialog->stSheepsBoxDataTotals->append(QString::number(stInSheepTotals));
    gameStatsDialog->stSheepsBoxDataTotals->append(QString::number(stClonedSheepTotals));
    gameStatsDialog->stSheepsBoxDataTotals->append(QString::number(stOutSheepTotals));
    gameStatsDialog->stSheepsBoxDataTotals->append(QString::number(stMaxSheepEvolution));

    // dels llops
    gameStatsDialog->stWolvesBoxDataValues->clear();
    gameStatsDialog->stWolvesBoxDataValues->append(QString::number(stLiveWolves));
    gameStatsDialog->stWolvesBoxDataValues->append(QString::number(stInWolves));
    gameStatsDialog->stWolvesBoxDataValues->append(QString::number(stClonedWolves));
    gameStatsDialog->stWolvesBoxDataValues->append(QString::number(stOutWolves));
    gameStatsDialog->stWolvesBoxDataValues->append(QString::number(stWolfEvolution));
    // totals de l'iteració actual
    gameStatsDialog->stWolvesBoxDataTotals->clear();
    gameStatsDialog->stWolvesBoxDataTotals->append("totals");
    gameStatsDialog->stWolvesBoxDataTotals->append(QString::number(stInWolfTotals));
    gameStatsDialog->stWolvesBoxDataTotals->append(QString::number(stClonedWolfTotals));
    gameStatsDialog->stWolvesBoxDataTotals->append(QString::number(stOutWolfTotals));
    gameStatsDialog->stWolvesBoxDataTotals->append(QString::number(stMaxWolfEvolution));
}

