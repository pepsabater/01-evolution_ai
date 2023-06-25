#include "compass.h"

Compass::Compass()
{
xPos=0;   // posició a l'eix de les X
yPos=0;   // posició a l'eix de les Y
sense=COMPASS_N;  // orientació de la brúxula, N, E, S, O, etc...
compassAngle=0;   // angle actual aliniat amb la brúxola
driftAngle=0;     // angle de correció per operacions de gir
}

void Compass::pointTo()
{
int targetAngle=0;  // angle on destinació
    // Correcció a fer
    // positiu = en sentit de les agulles del rellotge
    // negatiu = en sentit contrari a les agulles del rellotge
    // aliniem l'angle de la brúxola
    targetAngle=sense*(360/COMPASS_STEPS);
    // calculem la correcció
    driftAngle=targetAngle-compassAngle;
    // adjustem l'apuntador angular
    compassAngle=targetAngle;
}
