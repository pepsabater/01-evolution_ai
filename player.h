#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QGraphicsRectItem>
#include "compass.h"

// si belluga i interacciona directament
#define PLAYER_MODE_STATIC    0
#define PLAYER_MODE_DYNAMIC   1

// l'ordre es fa servir per establir la jerarquia
// només es pot consumir els de 1 nivell inferior
// amb els del mateix nivell es pot reproduir (clonar)
// amb els de 1 nivell superior es perd energia

#define PLAYER_TYPE_APPLE     0
#define PLAYER_TYPE_SHEEP     1
#define PLAYER_TYPE_WOLF      2
#define PLAYER_TYPES          3

#define PLAYER_STATUS_DEAD    0   // és mort
#define PLAYER_STATUS_LIVE    1   // és viu

// accions i esdeveniments
#define PLAYER_ACTION_WAIT    0     // esperar
#define PLAYER_ACTION_WALK    1     // caminar
#define PLAYER_ACTION_LEFT    2     // tombrar a l'esquerra
#define PLAYER_ACTION_RIGHT   3     // tombar a la dreta
#define PLAYER_ACTION_EATING  4     // menjar
#define PLAYER_ACTION_EATEN   5     // ser menjat
#define PLAYER_ACTION_FLEE    6     // fugir
#define PLAYER_ACTION_PURSUE  7     // perseguir
#define PLAYER_ACTION_CLONE   8     // reproduir-se
#define PLAYER_ACTIONS        9     // nombre d'accions esdeveniments

class Player 
{
public:
// característiques i estats
int type;           // de quina mena és
int evolution;      // generació a la que pertany
int status;         // estat
int empathy;        // nivell d'afinitat amb els seus semblants, de 0 a 9
int bravery;        // nivell de valentia davant els seus contraris, de 0 a 9
int pairs;          // nombre d'interaccions amb iguals,
                    // en arrivar al topall entra en modus 'clone'
bool active;      // si és dinàmic o estàtic
bool clonable;    // si está en condicions de clonar-se
bool onPlay;

// posicionament i orientació
Compass compass;

// energia
int energy;             // energia actual
int energyDrainOnClone; // energia que perd al reproduir-se
int energyDrainOnMove;  // energia que perd quan belluga
int energyToLost;       // energia a perdre
int energyToGain;       // energia a guanyar

// clonatge
int cloneOnPairs;        // quan clona

// estadístiques
int stEnergyWin;       // energia obtinguda
int stEnergyLost;      // energia gastada
int stWaitsTaken;      // cops que s'ha aturat
int stStepsTaken;      // passes fetes
int stTimesCloned;     // cops que ha clonat
int stTotalPairs;      // interaccions totals

// imatge associada
QGraphicsPixmapItem* pixmapImage;

    void updateEnergy();
    bool updateAction (int);

    Player();
    virtual Player* Clone()=0;
    virtual Player* Raise()=0;

    // característiques i estats   
    int getType(){return type;};
    int getEvolution(){return evolution;};
    int getStatus(){return status;};
    int getPairs(){return pairs;};
    bool isActive(){return active;};
    bool isClonable(){return clonable;};    // si está en condicions de clonar-se
    bool isOnPlay(){return onPlay;};        // si hi és a escena...
    // energia
    int getDrainOnPair(){return energyDrainOnClone;};
    int getDrainOnMove(){return energyDrainOnMove;};
    // relacions...
    int getEmpathy(){return empathy;};
    int getBravery(){return bravery;};
    // imatge
    void setPixmap(QGraphicsPixmapItem* qpixmap){pixmapImage=qpixmap;};
    QGraphicsPixmapItem* getPixmap(){return pixmapImage;};

    // es important aturar-lo perque si no ho fem quan vol esborrar-se del'escena
    // potser l'ha canviat de lloc i peta!!
    void playerPlayOff(){active=onPlay=false;};

    // energia
    int getEnergy(){return energy;};
    int getDrainEnergy(){return energyDrainOnClone;};

    // posicionament i orientació
    void pointTo(){compass.pointTo();};
    int getCompassAngle(){return compass.getCompassAngle();};
    void setPos(int xpos, int ypos){compass.setXPos(xpos);compass.setYPos(ypos);};
    void setXPos (int xpos){compass.setXPos(xpos);};
    int getXPos(){return compass.getXPos();};
    void setYPos (int ypos){compass.setYPos(ypos);};
    int getYPos(){return compass.getYPos();};
    void setCompass(int direction){compass.setSense(direction);};
    int getCompass(){return compass.getSense();};

    // accions i esdeveniments
    void playerAction(int paction){updateAction(paction);};
    void playerMoveTo(int xpos, int ypos){setPos(xpos, ypos); updateAction(PLAYER_ACTION_WALK);};
    void playerEating(int newEnergy){energyToGain+=newEnergy; updateAction(PLAYER_ACTION_EATING);};
    void playerEaten(int lostEnergy){energyToLost+=lostEnergy; updateAction(PLAYER_ACTION_EATEN);};
    void playerGainEnergy(int gainEnergy){energyToGain+=gainEnergy; updateAction(PLAYER_ACTION_EATING);};
    void playerDrainEnergy(int lostEnergy){energyToLost+=lostEnergy; updateAction(PLAYER_ACTION_EATEN);};
    void playerPair();
};

#endif // PLAYER_H
