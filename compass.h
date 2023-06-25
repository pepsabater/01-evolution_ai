#ifndef COMPASS_H
#define COMPASS_H

// orientació, cap a on es mou i/o interacciona
#define NO_COMPASS      (-1)
#define COMPASS_N       0
#define COMPASS_NE      1
#define COMPASS_E       2
#define COMPASS_SE      3
#define COMPASS_S       4
#define COMPASS_SW      5
#define COMPASS_W       6
#define COMPASS_NW      7
#define COMPASS_STEPS   8

class Compass
{
int xPos;   // posició a l'eix de les X
int yPos;   // posició a l'eix de les Y
int sense;  // orientació de la brúxula, N, E, S, O, etc...
int compassAngle;   // angle actual aliniat amb la brúxola
int driftAngle;     // angle de correció per operacions de gir

public:
    Compass();
    void pointTo(); //  aliniar
    void setXPos(int xpos){xPos=xpos;};
    int getXPos(){return xPos;};
    void setYPos(int ypos){yPos=ypos;};
    int getYPos(){return yPos;};
    void setPos(int xpos, int ypos){xPos=xpos; yPos=ypos;};
    void setSense(int isense){sense=isense;};
    int getSense(){return sense;};
    int getCompassAngle(){return compassAngle;};
    int getDriftAngle(){return driftAngle;};
};

#endif // COMPASS_H
