#include "global.h"
#include "gamecontrol.h"

GameController::GameController(QGraphicsScene* scene, GameView* view, MyStatsDialog* statsDialog)
{   // assignem l'escena i la vista heretades
    gameScene=scene;
    gameView=view;
    gameStatsDialog=statsDialog;

    // connectem els events de teclat i un temporitzador a la classe
    connect(view, SIGNAL(keyPressEvent(QKeyEvent*)), this, SLOT(gOnKeys(QKeyEvent*)));
    gameTimer = new QTimer();
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gOnTimer()));
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
    gameTimer->setInterval(GAME_WORLD_TIMER_TIME);
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

            if (gPlayerAdd (newplayer)== true)
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
            newplayer->setVision(PLAYER_FULL_VISION);

            if (gPlayerAdd (newplayer)==true)
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
            newplayer->setVision(PLAYER_SEMI_VISION);

            if (gPlayerAdd (newplayer)==true)
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
void GameController::gOnTimer()
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

                    if (newPlayer!=nullptr && gPlayerAdd(newPlayer)==true)
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

                    if (newPlayer!=nullptr && gPlayerAdd (newPlayer)==true)
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
    gPlayersMotion();
    // analisi de topades
    gSceneCollisions();
    // visualitzar estadístiques
    gGameStatistics();
    gPlayersStatistics();

    // si ha mort tota una espècie!
    if (stLiveSheeps==0 || stLiveWolves==0)
    { // aturem el món!
        gameTimer->stop();
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

void GameController::gPlayersMotion()
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

//----------------------------------------
// afegueix el jugador i l'hi busca lloc
//----------------------------------------
// fa servir el vector de jugadors
bool GameController::gPlayerAdd(Player* newplayer)
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

void GameController::gOnKeys (QKeyEvent* keyEvent)
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
        (gameTimer->isActive()==true)? gameTimer->stop() : gameTimer->start();
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


void GameController::gSceneCollisions()
{   // es validen els contactes del jugador 0
int oncontactplayer=PLAYER_NONE;
int onfocusplayer=PLAYER_NONE;
int counter=0;

    for (counter = 0; (unsigned)counter < playersPool.size(); counter++)
    {
        oncontactplayer=playerOnContact(playersPool[counter]);

        if (oncontactplayer != PLAYER_NONE
            && playersPool[oncontactplayer]->isOnPlay() == true
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

            if (onfocusplayer != PLAYER_NONE
                && playersPool[onfocusplayer]->isOnPlay() == true)
            {
                if (playersPool[counter]->isFleeing() == false)
                    switch (playersPool[counter]->getType() - playersPool[onfocusplayer]->getType())
                    {
                    case -1:    // és un depredador, fugim
                        playersPool[counter]->isFleeing(true);
                        playersPool[counter]->pointTo(playersPool[onfocusplayer]->getXPos(), playersPool[onfocusplayer]->getYPos(), false);
                        playerRotation(playersPool[counter]);
                        break;
                    case 0: // és un 'igual'
                        playersPool[counter]->isHunting(false);
                        break;
                    case 1: // és menjar, el cerquem
                        playersPool[counter]->isHunting(true);
                        playersPool[counter]->pointTo(playersPool[onfocusplayer]->getXPos(), playersPool[onfocusplayer]->getYPos(), true);
                        playerRotation(playersPool[counter]);
                        break;
                    }
            }
            else
            {
                playersPool[counter]->isFleeing(false);
                playersPool[counter]->isHunting(false);
            }
        }
    }
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

