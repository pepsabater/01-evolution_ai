#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include "global.h"
#include "mygameview.h"
#include "mystatsdialog.h"
#include "apple.h"
#include "sheep.h"
#include "wolf.h"

class GameController: public QObject
{
protected:

    Q_OBJECT

QGraphicsScene* gameScene;
GameView* gameView;
MyStatsDialog* gameStatsDialog;
QPixmap* gameBackImage;
QBrush* gameBrush;
QTimer* gameTamTam;

// estadístiques
int stIteration=0;
int stLiveApples=0;   // vives
int stOutApples=0;    // que han mort
int stInApples=0;     // incorporades al joc

int stInAppleTotals=0;     // incorporades al joc
int stOutAppleTotals=0;    // que han mort

int stLiveSheeps=0;
int stInSheeps=0;
int stClonedSheeps=0;
int stOutSheeps=0;
int stSheepEvolution=0;

int stInSheepTotals=0;
int stClonedSheepTotals=0;
int stOutSheepTotals=0;
int stMaxSheepEvolution=0;

int stLiveWolves=0;
int stInWolves=0;
int stClonedWolves=0;
int stOutWolves=0;
int stWolfEvolution=0;

int stInWolfTotals=0;
int stClonedWolfTotals=0;
int stOutWolfTotals=0;
int stMaxWolfEvolution=0;

int stLastSurvivor=0;

Player* gamePlayer0=nullptr;
Player* lastSheep=nullptr;
Player* lastWolf=nullptr;

QString backPixmapPath="../imatges/background20.png";
QString applePixmapPath="../imatges/red-apple20.png";
QString skullPixmapPath="../imatges/dead-skull20.png";

QString sheepPixmapPaths[SHEEP_PIXMAPS]={
    "../imatges/sheep-grey20.png",
    "../imatges/sheep-white20.png",
    "../imatges/sheep-yellow20.png",
    "../imatges/sheep-orange20.png",
    "../imatges/sheep-red20.png",
    "../imatges/sheep-purple20.png",
    "../imatges/sheep-blue20.png",
    "../imatges/sheep-green20.png",
    };

QString wolfPixmapPaths[WOLF_PIXMAPS]={
    "../imatges/wolf-grey20.png",
    "../imatges/wolf-white20.png",
    "../imatges/wolf-yellow20.png",
    "../imatges/wolf-orange20.png",
    "../imatges/wolf-red20.png",
    "../imatges/wolf-purple20.png",
    "../imatges/wolf-blue20.png",
    "../imatges/wolf-green20.png"
    };

QString survivorPlayerName[PLAYER_TYPES]={
    "none",
    "sheeps",
    "wolves"
};

std::vector<Player*>playersPool;   // col·lecció de jugadors

    // actualitzar les estadístiques
    void gameStatistics();
    // afegir jugador
    bool playerAdd(Player*);
    // desplaçar jugador segons el compàs
    bool playerMove(Player*);
    // dur el jugador a l'esquerra
    void playerGoLeft(Player*);
    // dur el jugador a la dreta
    void playerGoRight(Player*);
    // dur el jugador a l'inrevés
    void playerGoReverse(Player*);
    // trobada de jugadors, jugador i punt de trobada (x, y)
    void playersParty(Player*, Player*);
    // topada entre jugadors
    void playersDance(Player*, Player*);
    // l'un menja l'altre
    void playersLunch(Player*, Player*);
    // dibuixem el jugador
    void playerDraw(Player*, bool);
    // torna el jugador que es troba en un lloc
    Player* playerWhoIs(int, int);
    // busca el primer jugador de la llista de la mena sol·licitada
    Player* playerFinder(int);
    // comptador de jugadors segons la mena i l'estat
    int playerCounter(int, int);

public:

    GameController(QGraphicsScene*, GameView*, MyStatsDialog*);
    void gStartTamTam(){gameTamTam->start();}
    void gSetInit();
    void gSetRestart();
    void gSetTheatre();
    void gMakePlayers(int); // la mena de jugadors, bens o llops
    void gGameStatistics(); // estadístiques del joc
    void gPlayersStatistics();     // estadístiques del joc
    void gPlayersTotals();         // acumulats del joc

public slots:
    // connectors events->mètodes
    // gestor d'actualizació del joc en temps real
    void onTamTam();
    // gestor de teclat, events de teclat
    void touchMe(QKeyEvent*);
};

#endif // GAMECONTROL_H
