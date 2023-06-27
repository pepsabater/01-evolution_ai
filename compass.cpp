#include "compass.h"
#include <math.h>

Compass::Compass()
{
    xPos=0;   // posició a l'eix de les X
    yPos=0;   // posició a l'eix de les Y
    sense=COMPASS_N;  // orientació de la brúxula, N, E, S, O, etc...
    compassAngle=0;   // angle actual aliniat amb la brúxola

}

void Compass::pointTo()
{   // aliniem l'angle de la brúxola
    compassAngle=sense*(360/COMPASS_STEPS);
}

void Compass::pointTo(int xtarget, int ytarget, bool tooragainst)
{
double height=0.0;  // alçada del triangle rectàngle
double width=0.0;   // base del triangle
double angle=0.0;   // és l'angle que busquem en graus
int dummy1=0;

    // calcular l'angle de diferència a partir de la posició actual
    // calculem els costats del triangle respecte a l'a vertical'horitzontal
    width=xtarget-xPos;
    height=ytarget-yPos;

    // apliquem l'arctangent i la conversió de radiants a graus
    angle=(std::atan2(height, width)) * (180/PI);
    // corregim l'angle per que la nostra referència dels 0 graus assenyala al nord
    if (tooragainst == true)
        angle+=90;
    else
        angle-=90;
    // convertim l'angle objectiu a les unitats equivalents de la bruíxola, 45 graus
    // aliniem l'angle de la brúxola
    dummy1=(int)angle / (360/COMPASS_STEPS);

    // correcció de 'pífies'
    if ((int)angle%(360/COMPASS_STEPS)>(360/COMPASS_STEPS)/2)
        sense=dummy1 + 1;
    else
        sense=dummy1;

    if(sense<0)
        sense+=COMPASS_STEPS;

    if(sense == COMPASS_STEPS)
        sense=COMPASS_N;

    compassAngle=sense*(360/COMPASS_STEPS);
}

