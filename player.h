#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QGraphicsRectItem>
#include "compass.h"

// si belluga i interacciona directament
#define PLAYER_MODE_STATIC    0
#define PLAYER_MODE_DYNAMIC   1

#define PLAYER_STATUS_DEAD    0   // és mort
#define PLAYER_STATUS_LIVE    1   // és viu

// l'ordre es fa servir per establir la jerarquia
// només es pot consumir els de 1 nivell inferior
// amb els del mateix nivell es pot reproduir (clonar)
// amb els de 1 nivell superior es perd energia

#define PLAYER_TYPE_APPLE   0
#define PLAYER_TYPE_SHEEP   1
#define PLAYER_TYPE_WOLF    2
#define PLAYER_TYPES        3
#define PLAYER_NONE         -1

// accions i esdeveniments
#define PLAYER_ACTION_GO        0   // anar endavant
#define PLAYER_ACTION_TLEFT     1   // tombar a esquerra
#define PLAYER_ACTION_TRIGHT    2   // tombar a dreta
#define PLAYER_ACTION_REVERSE   3   // girar cua
#define PLAYER_ACTION_WAIT      4   // esperar
#define PLAYER_ACTION_PURSUE    5   // perseguir
#define PLAYER_ACTION_FLEE      6   // fugir
#define PLAYER_ACTION_FIGHTH    7   // luitar/defensar-se
#define PLAYER_ACTION_PAIR      8   // aparellar-se
#define PLAYER_ACTION_EATING    9   // menjar
#define PLAYER_ACTIONS          10  // nombre d'accions esdeveniments

// paràmetres de l'ull
#define PLAYER_FULL_VISION      0
#define PLAYER_SEMI_VISION      1
#define PLAYER_FVISION_RADIUS 160
#define PLAYER_SVISION_RADIUS 240
#define PLAYER_VISION_START    60
#define PLAYER_VISION_SPAN     60

struct stPlayerVision
{   // modus de visió
QGraphicsEllipseItem* item=nullptr;
QColor* color=nullptr;
QBrush* brush=nullptr;
int radius=PLAYER_FVISION_RADIUS;
int startAngle=PLAYER_VISION_START;
int spanAngle=PLAYER_VISION_SPAN;
};

struct stPlayerAspect
{   // imatge associada
QGraphicsPixmapItem* pixmap=nullptr;
QString pixmapPath="";
};

class Player 
{
protected:
// característiques i estats
int type;       // de quina mena és
int evolution;  // generació a la que pertany
int status;     // estat
int empathy;    // nivell d'afinitat amb els seus semblants, de 0 a 9
int bravery;    // nivell de valentia davant els seus contraris, de 0 a 9
int pairs;      // nombre d'interaccions amb iguals,
                // en arrivar al topall entra en modus 'clone'
bool active;    // si és dinàmic o estàtic
bool clonable;  // si está en condicions de clonar-se
bool hunting;   // si està caçant
bool fleeing;   // si està fugint
bool onPlay;    // si hi és en actiu dins el joc

// posicionament i orientació
Compass compass;

// aparença i visió
struct stPlayerAspect playerFace;
struct stPlayerVision playerVision;

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

public:

    Player();
    ~Player();
    virtual Player* Clone()=0;
    virtual Player* Raise()=0;

    // característiques i estats   
    int getType(){return type;};
    int getEvolution(){return evolution;};
    int getStatus(){return status;};
    bool isHunting(){return hunting;};
    void isHunting(bool ishunting){hunting=ishunting;};
    bool isFleeing(){return fleeing;};
    void isFleeing(bool isfleeing){fleeing=isfleeing;};
    int getPairs(){return pairs;};
    bool isActive(){return active;};
    bool isClonable(){return clonable;};    // si está en condicions de clonar-se
    bool isOnPlay(){return onPlay;};        // si hi és a escena...
    // energia
    int getDrainOnPair(){return energyDrainOnClone;};
    int getDrainOnMove(){return energyDrainOnMove;};
    void updateEnergy();
    int getEnergy(){return energy;};
    int getDrainEnergy(){return energyDrainOnClone;};
    // relacions...
    int getEmpathy(){return empathy;};
    int getBravery(){return bravery;};
    // aparença
    void setPixmapPath(QString pixpath){playerFace.pixmapPath=pixpath;};
    QString getPixmapPath(){return playerFace.pixmapPath;};
    void setPixmap(QGraphicsPixmapItem* qpixmap){playerFace.pixmap=qpixmap;};
    QGraphicsPixmapItem* getPixmap(){return playerFace.pixmap;};

    // es important aturar-lo perque si no ho fem quan vol esborrar-se del'escena
    // potser l'ha canviat de lloc i peta!!
    void playerPlayOff(){active=onPlay=false;};

    // posicionament i orientació
    void setPos(int, int);
    void setXPos (int xpos){compass.setXPos(xpos);};
    int getXPos(){return compass.getXPos();};
    void setYPos (int ypos){compass.setYPos(ypos);};
    int getYPos(){return compass.getYPos();};
    void setCompass(int direction){compass.setSense(direction);};
    int getCompass(){return compass.getSense();};
    int getCompassAngle(){return compass.getCompassAngle();};
    void alignTo(int);
    void pointTo(){compass.pointTo();};
    void pointTo(int x, int y, bool tooragainst){compass.pointTo(x, y, tooragainst);};

    // camp de visió
    void setVision(int);
    int getVisionRadius(){return playerVision.radius;};
    int getVisionSpanAngle(){return playerVision.spanAngle;};
    void setVisionRadius(int visionradius){playerVision.radius=visionradius;};
    void setVisionItem(QGraphicsEllipseItem* visionitem){playerVision.item=visionitem;};
    QGraphicsEllipseItem* getVisionItem(){return playerVision.item;};

    // accions i esdeveniments
    bool updateAction (int);
    void playerAction(int paction){updateAction(paction);};
    void playerMoveTo(int xpos, int ypos){setPos(xpos, ypos); updateAction(PLAYER_ACTION_GO);};
    void playerEating(int newEnergy){energyToGain+=newEnergy; updateAction(PLAYER_ACTION_EATING);};
    void playerGainEnergy(int gainEnergy){energyToGain+=gainEnergy; updateAction(PLAYER_ACTION_EATING);};
    void playerDrainEnergy(int lostEnergy){energyToLost+=lostEnergy; updateAction(PLAYER_ACTION_EATING);};
    void playerPair();
};

#endif // PLAYER_H
