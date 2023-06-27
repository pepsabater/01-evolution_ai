#ifndef GLOBAL_H
#define GLOBAL_H

// de Qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsScale>
#include <QtWidgets/QDialog>
#include <QIcon>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QTextEdit>
#include <QKeyEvent>
#include <QTimer>

// genèrics de C++
#include <vector>
#include <array>
#include <string>

// de l'aplicació
#include "player.h"

// paràmetres gràfics
#define VIEW_WIDTH      1440    // 1440 amplada de la pantalla de joc
#define DIALOG_WIDTH    320     // 240 amplada del diàleg
#define VIEW_HEIGHT     960     // 800 alçada de la pantalla de joc
#define PIXMAP_WIDTH    20      // amplada de les icones dels jugadors
#define PIXMAP_HEIGHT   20      // alçada de les icones dels jugadors
#define PIXMAP_XFOCUS   (PIXMAP_WIDTH)    //((PIXMAP_WIDTH/2))    // per a la detecció dels jugadors dins l'escena
#define PIXMAP_YFOCUS   (PIXMAP_HEIGHT)  //((PIXMAP_HEIGHT/2))   // per a la detecció dels jugadors dins l'escena
#define PLAYER_XDRIFT   2 // 10 desplaçament a l'eix X
#define PLAYER_YDRIFT   2 // 10 desplaçament a l'eix Y
#define SHEEP_PIXMAPS   8       // n. d'icones a la col·lecció d'icones
#define WOLF_PIXMAPS    8       // n. d'icones a la col·lecció d'icones

// temps d'autoactualització
#define GAME_WORLD_TIMER_TIME   30    // 100

// ecosistema del 'joc'
#define INITIAL_APPLES  32  // 32
#define MAX_APPLES      48  // 48
#define INITIAL_SHEEPS  12  // 12
#define MAX_SHEEPS      24  // 24
#define INITIAL_WOLVES  6   // 6
#define MAX_WOLVES      12  // 12

// paràmetres d'energia
#define APPLE_MAX_ENERGY  250   // 100
#define SHEEP_MAX_ENERGY  800    // 500
#define WOLF_MAX_ENERGY   1000   // 700

// energia consumida a cada acció
#define APPLE_DRAINED_ENERGY        25   // quan és 'mossegada'
#define SHEEP_DRAINED_ENERGY        100  // quan hi ha una acció rellevant (topada, clonatge)
#define WOLF_DRAINED_ENERGY         60   // quan hi ha una acció rellevant (topada, clonatge)
#define PLAYER_DRAINED_ONMOVE       5    // quan belluga
#define PLAYER_DRAINED_ONFLEE       (PLAYER_DRAINED_ONMOVE*5)

#define SHEEP_CLONE_ON  3    // contactes mínims per donar-lo per clonable
#define WOLF_CLONE_ON   2    // contactes mínims per donar-lo per clonable

// intel·ligència virtual ;-)
#define PLAYER_BRAIN_CAPES    3   // bàsic (1 input,1 hide,1 output)
    // capa d'entrada
    // 2 si tenim contacte i de quina mena
    // 3 d'energia (actual i les posibles pèrdues)
    // 3 per posició i orientació
#define PLAYER_BRAIN_INPUTS   8
    // capa amagada
    // 1 contacte
    // 1 energia
    // 1 moviment
    // 1 accions
#define PLAYER_BRAIN_HIDE     4
    // capa de sortida
    // 4 pel moviment (N,S,E,O)
    // 8 per les accions
#define PLAYER_BRAIN_OUTPUTS  12
    // creixement del cervell
#define PLAYER_BRAIN_GROW_A   1   // com creix la capa amagada
#define PLAYER_BRAIN_GROW_G   20  // cada quantes generacions creix

// generador de números aleatoris (min, max, jump)
int alNumGen (int, int, int);

#endif // GLOBAL_H
