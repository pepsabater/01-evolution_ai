#include "global.h"
#include "mygameview.h"
#include "mystatsdialog.h"
#include "gamecontrol.h"

int main(int argc, char *argv[])
{
GameController* gameControl;

    // crea l'aplicació Qt
    // cal fer-ho abans de les inicialitzacions per dependències internes a les classes de Qt
    QApplication gameApp(argc, argv);
    QIcon icon("../imatges/evolution_ai80.png");
    gameApp.setWindowIcon(icon);

    // finestra principal
    QDialog gameWindow;
    // elimina les vores de la finestra principal
    gameWindow.setFixedWidth(VIEW_WIDTH+DIALOG_WIDTH);
    gameWindow.setFixedHeight(VIEW_HEIGHT);
    gameWindow.setStyleSheet("background-color:black;");
    gameWindow.setWindowTitle("evolution AI");

    // crea el diàleg de les estadístiques
    MyStatsDialog statsDialog(&gameWindow);
    statsDialog.setAutoFillBackground(true);
    statsDialog.setFixedWidth(DIALOG_WIDTH);
    statsDialog.setFixedHeight(VIEW_HEIGHT);

    // inicialitzem el diàleg
    statsDialog.dInit();

    // crea una escena, una vista gràfica i les associa
    QGraphicsScene gameScene;
    GameView gameView;

    /* -------------------------------------------------------------------------------------------------------
     *  COMPTE!!!: hi ha un 'bug', lligat a l'esborrat d'elements de l'escena, que es sobrepassa deshabilitant
     * el sistema d'indexació intern anomenat 'BSP', que fa petar l'escena quan es redibuixa
    ------------------------------------------------------------------------------------------------------- */
    gameScene.setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);

    // creació i configuració de la disposició de la finestra i les divisions
    QSplitter gameSplitter(&gameWindow);
    gameSplitter.addWidget(&gameView);
    gameSplitter.addWidget(&statsDialog);

    // crea el control de joc, l'inicialitza i el presenta
    gameControl = new GameController(&gameScene, &gameView, &statsDialog);
    gameControl->gSetInit();
    gameControl->gSetTheatre();
    // mostra la finestra
    gameWindow.show();

    return gameApp.exec();
}

