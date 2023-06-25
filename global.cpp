#include "global.h"
#include <iostream>
#include <random>
#include <ctime>

static bool seeded=false;

// generador de números aleatoris
int alNumGen (int min, int max, int step)
{
    if (seeded==false)
    {   // generem la llavor pels valors aleatoris
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded=true;
    }
    return min+((std::rand() % ((max-min+1)/step))*step);
}

//---------------------------------------------
// comprova si un punt és dins d'un rectangle
//---------------------------------------------
// x,y           : punt a verificar
// startx,starty : inici del rectàngle
// width,height  : dimensions del rectàngle
// margin        : marge del rectàngle a avaluar
bool isPointInRect (int x, int y, int startx, int starty, int width, int height, int margin)
{
int xmin;
int ymin;
int xmax;
int ymax;
bool retval=false;

    xmin=startx-(width/2)+margin;
    ymin=starty-(height/2)+margin;
    xmax=startx+width+(width/2)-margin;
    ymax=starty+height+(height/2)-margin;

    if ((x >= xmin && x <= xmax)
        && (y >= ymin && y <= ymax))
        retval = true;
    return retval;
}

