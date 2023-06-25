#include "global.h"
#include "gamecontrol.h"

GameController::GameController(QGraphicsScene* scene, GameView* view, MyStatsDialog* statsDialog)
{
    // assignem l'escena i la vista heretades
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
    gMakePlayers(PLAYER_TYPE_APPLE);
    gMakePlayers(PLAYER_TYPE_SHEEP);
    gMakePlayers(PLAYER_TYPE_WOLF);
}

void GameController::gMakePlayers(int playerType)
{
int counter;
Player* newPlayer=nullptr;

    switch (playerType)
    {
    case PLAYER_TYPE_APPLE:
        for (counter=0; counter<INITIAL_APPLES; counter++)
        {   // menjar
            newPlayer=new Apple();
            if (playerAdd (newPlayer)== true)
            {   // estadístiques
                stLiveApples++;
                stInApples++;
                stInAppleTotals++;
            }
        }
        break;
    case PLAYER_TYPE_SHEEP:
        for (counter=0; counter<INITIAL_SHEEPS; counter++)
        {   // bens, generats de cero o a partir del que ha trigat més en morir
            if(lastSheep!=nullptr)
                newPlayer=lastSheep->Clone();
            else
                newPlayer=new Sheep();

            if (playerAdd (newPlayer)==true)
            {   // estadístiques
                stLiveSheeps++;
                stInSheeps++;
                stInSheepTotals++;

                if(newPlayer->getEvolution()>stSheepEvolution)
                  stSheepEvolution=newPlayer->getEvolution();
            }
        }
        break;
    case PLAYER_TYPE_WOLF:
        for (counter=0; counter<INITIAL_WOLVES; counter++)
        {   // llops, generats de cero o a partir del que ha trigat més en morir
            if(lastWolf!=nullptr)
                newPlayer=lastWolf->Clone();
            else
                newPlayer=new Wolf();

            if (playerAdd (newPlayer)==true)
            {   // estadístiques
                stLiveWolves++;
                stInWolves++;
                stInWolfTotals++;

                if(newPlayer->getEvolution()>stWolfEvolution)
                  stWolfEvolution=newPlayer->getEvolution();
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
QGraphicsPixmapItem* currPlayerPixmap=nullptr;
int newApples=0;

    // posem a l'hora els comptadors de cicles
    growCounter++;
    lifeCounter++;

    // neteja de cadàvers
    for (counter=0; (unsigned long)counter < playersPool.size(); counter++)
    {   // neteja de cadàvers
        if(playersPool[counter]->getStatus()==PLAYER_STATUS_DEAD
            && playersPool[counter]->isOnPlay()== true)
        {   // el fotem fora del joc
            switch (playersPool[counter]->getType())
            {   // recollim estadístiques
            case PLAYER_TYPE_APPLE:
                stLiveApples--;
                stOutApples++;
                stOutAppleTotals++;
                break;
            case PLAYER_TYPE_SHEEP:
                lastSheep=playersPool[counter];
                stLiveSheeps--;
                stOutSheeps++;
                stOutSheepTotals++;
                break;
            case PLAYER_TYPE_WOLF:
                lastWolf=playersPool[counter];
                stLiveWolves--;
                stOutWolves++;
                stOutWolfTotals++;
                break;
            }
            // l'assenyalem com a fora del joc
            playersPool[counter]->playerPlayOff();
            currPlayerPixmap=playersPool[counter]->getPixmap();

            if (currPlayerPixmap != nullptr)
                gameScene->removeItem(currPlayerPixmap);
            playersPool[counter]->setPixmap(nullptr);
        }
    }

    // belluguem els jugadors
    for (counter=0; (unsigned long)counter < playersPool.size(); counter++)
    {   // movem els jugadors
        if (playersPool[counter]->isActive()==true
                && playersPool[counter]->getStatus() == PLAYER_STATUS_LIVE
                && playersPool[counter]->isOnPlay()==true)
        {   // forcem un moviment aleatori, per ara
            switch ((alNumGen(0, 3, 1)<3)? PLAYER_ACTION_WALK : alNumGen(1,3,1))
            {
            case PLAYER_ACTION_WAIT:    // esperar
                break;
            case PLAYER_ACTION_WALK:    // caminar
                playerMove(playersPool[counter]);
                break;
            case PLAYER_ACTION_LEFT:    // tombrar a l'esquerra
                playerGoLeft(playersPool[counter]);

                if (playerMove(playersPool[counter])==false)
                    playerGoLeft(playersPool[counter]);
                break;
            case PLAYER_ACTION_RIGHT:    // tombar a la dreta
                playerGoRight(playersPool[counter]);

                if (playerMove(playersPool[counter])==false)
                    playerGoRight (playersPool[counter]);
                break;
            default:
                break;
            }
        }
    }

    // cada 50 passes del rellotge (5 segons)
    if (growCounter == 50)
    {   // comptador de passes a zero
        growCounter = 0;

        //------------------------
        // regenerador de 'vida'
        //------------------------

        // les pomes, per cada 4 pomes 'vives' n'afegim una altra
        // si no n'hi ha cap regenerem la meitat de màxmin premés
        if (stLiveApples==0)
            newApples=MAX_APPLES/4;
        else
            if (stLiveApples + (unsigned int)stLiveApples/4 <= MAX_APPLES)
                newApples = (unsigned int)stLiveApples/4;
            else
                newApples = stLiveApples/4 - (stLiveApples + (unsigned int)stLiveApples/4 - MAX_APPLES);

        for (counter=0; counter < newApples; counter ++)
        {
            currPlayer=new Apple();
            if (playerAdd(currPlayer)==true)
            {   // estadístiques
                stLiveApples++;
                stInApples++;
                stInAppleTotals++;
            }
        }
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

    // visualitzar estadístiques
    gGameStatistics();
    gPlayersStatistics();

    // si ha mort tota una espècie!
    if (stLiveSheeps==0 || stLiveWolves==0)
    { // aturem el món!
//        gameTamTam->stop();
        // guanyador del joc
        if(stLiveSheeps==0||stLiveWolves==0)
        {
            stIteration++;
            stLastSurvivor=(stLiveSheeps==0)? PLAYER_TYPE_WOLF : PLAYER_TYPE_SHEEP;
            gSetRestart();
            gMakePlayers((stLiveSheeps==0)? PLAYER_TYPE_SHEEP : PLAYER_TYPE_WOLF);
        }
    }
}

//---------------------
// belluga el jugador
//---------------------

bool GameController::playerMove (Player* player)
{
int oldXPos;
int oldYPos;
int newXPos;
int newYPos;
QGraphicsPixmapItem* playerPixmap=nullptr;
QGraphicsPixmapItem* targetPixmap=nullptr;
Player* targetPlayer=nullptr;
bool retval=false;

    // si és un jugador vàlid
    if (player==nullptr)
        return retval;

    // enregistrem la seva posicio
    newXPos=oldXPos=player->getXPos();
    newYPos=oldYPos=player->getYPos();
    // mirem cap a on assenyala la bruïxola
    switch (player->getCompass())
    {   // i n'adjustem les noves coordinades
    case COMPASS_N:
        newYPos=oldYPos-PLAYER_XDRIFT;
        break;
    case COMPASS_NE:
        newXPos=oldXPos+PLAYER_XDRIFT;
        newYPos=oldYPos-PLAYER_YDRIFT;
        break;
    case COMPASS_E:
        newXPos=oldXPos+PLAYER_XDRIFT;
        break;
    case COMPASS_SE:
        newXPos=oldXPos+PLAYER_XDRIFT;
        newYPos=oldYPos+PLAYER_YDRIFT;
        break;
    case COMPASS_S:
        newYPos=oldYPos+PLAYER_YDRIFT;
        break;
    case COMPASS_SW:
        newXPos=oldXPos-PLAYER_XDRIFT;
        newYPos=oldYPos+PLAYER_YDRIFT;
        break;
    case COMPASS_W:
        newXPos=oldXPos-PLAYER_XDRIFT;
        break;
    case COMPASS_NW:
        newXPos=oldXPos-PLAYER_XDRIFT;
        newYPos=oldYPos-PLAYER_YDRIFT;
        break;
    default:    // no sap on vol anar!
        return retval;
    }

    // si la nova posició és dins l'escena
    if ((newXPos>=0 && newXPos <= VIEW_WIDTH-PIXMAP_WIDTH)
        && (newYPos>=0 && newYPos <= VIEW_HEIGHT-PIXMAP_HEIGHT))
    {   // enregistrem la referència de l'imatge a l'escena
        playerPixmap=player->getPixmap();

        if (playerPixmap != nullptr)
        {   // el posicionem a l'escena
            playerPixmap->setPos(newXPos,newYPos);
            // si no hi ha res
            if(gameScene->collidingItems(playerPixmap).size()==0)
            {
                // ho possem a l'hora
                player->playerMoveTo(newXPos,newYPos);
                player->playerAction(PLAYER_ACTION_WALK);
            }
            else
            {   // mirem amb que 'ha trobat
                targetPixmap=(QGraphicsPixmapItem*)gameScene->collidingItems(playerPixmap)[0];
                for (int counter=0; (unsigned int)counter < playersPool.size(); counter++)
                    if (targetPixmap == playersPool[counter]->getPixmap())
                        targetPlayer = playersPool[counter];
                // i després el tornem a lloc...
                player->setPos(oldXPos, oldYPos);
                playerPixmap->setPos(oldXPos,oldYPos);
                // i mirem que ha passat!
                playersParty(player, targetPlayer);
            }
            retval=true;
        }
    }
    return retval;
}

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
        playerEaten->playerEaten(playerEaten->getEnergy()); // consumim tota la seva energia, es mor
    }
}

//----------------------------------------
// afegueix el jugador i l'hi busca lloc
//----------------------------------------
// fa servir el vector de jugadors
bool GameController::playerAdd(Player* newPlayer)
{
QString facePixPath;
QGraphicsPixmapItem* facePixmap;

bool retval=false;

    if (newPlayer!=nullptr)
    {
        switch (newPlayer->getType())
        {   // carreguem l'imatge segons la mena de jugador
        case PLAYER_TYPE_APPLE:
            facePixPath=applePixmapPath;
            break;
        case PLAYER_TYPE_SHEEP:
            // canvia de color amb cada nova generació
            facePixPath=(newPlayer->getEvolution()>=SHEEP_PIXMAPS)? sheepPixmapPaths[newPlayer->getEvolution()%SHEEP_PIXMAPS]
                : sheepPixmapPaths[newPlayer->getEvolution()];
            break;
        case PLAYER_TYPE_WOLF:  // canvia de color amb cada nova generació
            // canvia de color amb cada nova generació
            facePixPath=(newPlayer->getEvolution()>=WOLF_PIXMAPS)? wolfPixmapPaths[newPlayer->getEvolution()%WOLF_PIXMAPS]
                : wolfPixmapPaths[newPlayer->getEvolution()];
            break;
        default:
            facePixPath=skullPixmapPath;
            break;
        }

        // creem i associem l'imatge
        facePixmap=new QGraphicsPixmapItem(QPixmap(facePixPath));
        facePixmap->setFlag(QGraphicsItem::ItemIsMovable, true);
        newPlayer->setPixmap(facePixmap);
        // afegim el jugador a la llista de jugadors
        playersPool.push_back(newPlayer);
        // el posicionem a l'escena
        facePixmap->setPos(newPlayer->getXPos(),newPlayer->getYPos());
        gameScene->addItem(facePixmap);

        do
        {   // el busquem un lloc a l'escena, repetim fins trobar-ne un de lliure
            newPlayer->setPos(alNumGen(0,VIEW_WIDTH-PIXMAP_WIDTH,PLAYER_XDRIFT),alNumGen(0,VIEW_HEIGHT-PIXMAP_HEIGHT,PLAYER_YDRIFT));
            facePixmap->setPos(newPlayer->getXPos(), newPlayer->getYPos());
        } while (gameScene->collidingItems(facePixmap).size()!=0);
        retval=true;
    }
    return retval;
}

//--------------------------------
// el jugador tomba a l'esquerra
//--------------------------------

void GameController::playerGoLeft(Player* player)
{
int leftDir;

    if (player!=nullptr)
    {
        leftDir=player->getCompass() - 1;
        player->setCompass((leftDir < 0)? COMPASS_STEPS - 1 : leftDir);
    }
}

//-------------------------------
// el jugador tomba a la dreta
//-------------------------------

void GameController::playerGoRight(Player* player)
{
int rightDir;

    if (player!=nullptr)
    {
        rightDir=player->getCompass() + 1;
        player->setCompass((rightDir == COMPASS_STEPS)? COMPASS_N : rightDir);
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
        player->setCompass((newDir == COMPASS_STEPS)? COMPASS_N : newDir);
    }
}

//-----------------------------------------------
// control del joc per teclat (per proves)
//-----------------------------------------------

void GameController::touchMe (QKeyEvent* keyEvent)
{
static Player* gamePlayer0=nullptr;

    if (gamePlayer0 == nullptr)
    {
        gamePlayer0=playerFinder(PLAYER_TYPE_SHEEP);
        if (gamePlayer0==nullptr)
            return;
    }
    switch (keyEvent->key())
    {
    case Qt::Key_Up:
        gamePlayer0->setCompass(COMPASS_N);
        break;
    case Qt::Key_Left:
        gamePlayer0->setCompass(COMPASS_W);
        break;
    case Qt::Key_Down:
        gamePlayer0->setCompass(COMPASS_S);
        break;
    case Qt::Key_Right:
        gamePlayer0->setCompass(COMPASS_E);
        break;
    case Qt::Key_W:
        gamePlayer0->setCompass(COMPASS_NW);
        break;
    case Qt::Key_E:
        gamePlayer0->setCompass(COMPASS_NE);
        break;
    case Qt::Key_S:
        gamePlayer0->setCompass(COMPASS_SW);
        break;
    case Qt::Key_D:
        gamePlayer0->setCompass(COMPASS_SE);
        break;
    case Qt::Key_Space:
        // per a tasques d'aprenentatge de l'AI
//            if (stLiveSheeps==0 || stLiveWolves==0)
//            {   // repetició i regeneració
//                stIteration++;
//                gSetRestart();
//                gMakePlayers((stLiveSheeps==0)? PLAYER_TYPE_SHEEP : PLAYER_TYPE_WOLF);
//            }
        // aturar/endegar el rellotge
        (gameTamTam->isActive()==true)? gameTamTam->stop() : gameTamTam->start();
        return;
    default:
        return;
    }
    playerMove(gamePlayer0);
}

//-----------------------------------------------
// dibuixem el jugador
//-----------------------------------------------
// si rotation = true, és que cal girar-lo
// si rotation = false, és un desplaçament lineal
//-----------------------------------------------

void GameController::playerDraw(Player *player, bool rotation)
{
QGraphicsPixmapItem* pixmapItem;

    pixmapItem=player->getPixmap();

    if (player==nullptr || player->getPixmap() == nullptr)
        return;

    if (rotation==true)
    {   // fixem el punt de gir fem la rotació
        pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
        pixmapItem->setRotation(player->getCompassAngle());
        pixmapItem->update();
    }
    else
        // el posicionem a l'escena
        pixmapItem->setPos(player->getXPos(), player->getYPos());
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

