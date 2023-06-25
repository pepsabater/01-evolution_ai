#include "global.h"
#include "mystatsdialog.h"

MyStatsDialog::MyStatsDialog(QWidget *parent) : QDialog(parent)
{
//public:

}

void MyStatsDialog::dInit()
{   // inicialitzem el diàleg
    //------------------------------------------------
    // contenidor principal del diàles d'estadístiques
    //------------------------------------------------
    QVBoxLayout* stMainBox = new QVBoxLayout(this);

    //-------------------------------------------
    // contenidor per a les dades globals del joc
    // ------------------------------------------
    QWidget* stGameWidget = new QWidget();
    QVBoxLayout* stGameBox = new QVBoxLayout(stGameWidget);
    // l'afegim al contenidor principal
    stMainBox->addWidget(stGameWidget);

    //-----------------------------------
    // contenidor per als jugadors 'poma'
    // ----------------------------------
    QWidget* stApplesWidget = new QWidget();
    QVBoxLayout* stApplesBox = new QVBoxLayout(stApplesWidget);
    // l'afegim al contenidor principal
    stMainBox->addWidget(stApplesWidget);

    //---------------------------------
    // contenidor per als jugadors 'bé'
    // --------------------------------
    QWidget* stSheepsWidget = new QWidget();
    QVBoxLayout* stSheepsBox = new QVBoxLayout(stSheepsWidget);
    // l'afegim al contenidor principal
    stMainBox->addWidget(stSheepsWidget);

    //-----------------------------------
    // contenidor per als jugadors 'llop'
    // ----------------------------------
    QWidget* stWolvesWidget = new QWidget();
    QVBoxLayout* stWolvesBox = new QVBoxLayout(stWolvesWidget);
    // l'afegim al contenidor principal
    stMainBox->addWidget(stWolvesWidget);

    //----------------------
    // contenidors de títols
    //----------------------

    // contenidor per al títol de les dades globals del joc
    //-----------------------------------------------------
    QWidget* stGameBoxWidget = new QWidget();
    stGameBoxWidget->setStyleSheet("background-color: white; color: black;");
    // organitzador horitzontal per a imatge + text
    QHBoxLayout* stGameBoxTitle = new QHBoxLayout(stGameBoxWidget);
    // imatge
    QLabel* stGameBoxTitleImage = new QLabel;
    QPixmap stGameBoxImage("../imatges/evolution_ai40.png");
    stGameBoxTitleImage->setPixmap(stGameBoxImage);
    // text
    QLabel* stGameBoxTitleText = new QLabel("evolution AI");
    // afegim la imatge i el text a l'organitzador del títol
    stGameBoxTitle->addWidget(stGameBoxTitleImage);
    stGameBoxTitle->addWidget(stGameBoxTitleText);
    // afegim el contenidor del títol al contenidor de dades del joc
    stGameBox->addWidget(stGameBoxWidget);

    // contenidor per al títol dels jugadors 'poma'
    //---------------------------------------------
    QWidget* stApplesBoxWidget = new QWidget();
    stApplesBoxWidget->setStyleSheet("background-color: white; color: black;");
    // organitzador horitzontal per a imatge + text
    QHBoxLayout* stApplesBoxTitle = new QHBoxLayout(stApplesBoxWidget);
    // imatge
    QLabel* stApplesBoxTitleImage = new QLabel;
    QPixmap stApplesBoxImage("../imatges/red-apple40.png");
    stApplesBoxTitleImage->setPixmap(stApplesBoxImage);
    // text
    QLabel* stApplesBoxTitleText = new QLabel("apples");
    // afegim la imatge i el text a l'organitzador del titol
    stApplesBoxTitle->addWidget (stApplesBoxTitleImage);
    stApplesBoxTitle->addWidget(stApplesBoxTitleText);
    // afegim el contenidor del títol al contenidor de dades
    stApplesBox->addWidget(stApplesBoxWidget);

    // contenidor per al títol dels jugadors 'bé'
    //-------------------------------------------
    QWidget* stSheepsBoxWidget = new QWidget();
    stSheepsBoxWidget->setStyleSheet("background-color: white; color: black;");
    // organitzador horitzontal per a imatge + text
    QHBoxLayout* stSheepsBoxTitle = new QHBoxLayout(stSheepsBoxWidget);
    // imatge
    QLabel* stSheepsBoxTitleImage = new QLabel;
    QPixmap stSheepsBoxImage("../imatges/sheep-white40.png");
    stSheepsBoxTitleImage->setPixmap(stSheepsBoxImage);
    // text
    QLabel* stSheepsBoxTitleText = new QLabel("sheeps");
    // afegim la imatge i el text a l'organitzador del titol
    stSheepsBoxTitle->addWidget (stSheepsBoxTitleImage);
    stSheepsBoxTitle->addWidget(stSheepsBoxTitleText);
    // afegim el contenidor del títol al contenidor de dades
    stSheepsBox->addWidget(stSheepsBoxWidget);

    // contenidor per al títol dels jugadors 'llop'
    //---------------------------------------------
    QWidget* stWolvesBoxWidget = new QWidget();
    stWolvesBoxWidget->setStyleSheet("background-color: white; color: black;");
    // organitzador horitzontal per a imatge + text
    QHBoxLayout* stWolvesBoxTitle = new QHBoxLayout(stWolvesBoxWidget);
    // imatge
    QLabel* stWolvesBoxTitleImage = new QLabel;
    QPixmap stWolvesBoxImage("../imatges/wolf-red40.png");
    stWolvesBoxTitleImage->setPixmap(stWolvesBoxImage);
    // text
    QLabel* stWolvesBoxTitleText = new QLabel("wolves");
    // afegim la imatge i el text a l'organitzador del titol
    stWolvesBoxTitle->addWidget (stWolvesBoxTitleImage);
    stWolvesBoxTitle->addWidget(stWolvesBoxTitleText);
    // afegim el contenidor del títol al contenidor de dades
    stWolvesBox->addWidget(stWolvesBoxWidget);

    //---------------------
    // contenidors de dades
    //---------------------

    // contenidor per a les dades generals del joc
    //--------------------------------------------
    QWidget* stGameBoxDataWidget = new QWidget();
    // organitzador horitzontal per a les dades
    QHBoxLayout* stGameBoxData = new QHBoxLayout(stGameBoxDataWidget);
    // dades a mostrar
    QTextEdit* stGameBoxDataLabels = new QTextEdit();
    // etiquetes
    stGameBoxDataLabels->setStyleSheet("background-color: black; color: white;");
    stGameBoxDataLabels->setReadOnly(true);
    stGameBoxDataLabels->append("iteration");
    stGameBoxDataLabels->append("in field");
    stGameBoxDataLabels->append("borns");
    stGameBoxDataLabels->append("cloned");
    stGameBoxDataLabels->append("deads");
    stGameBoxDataLabels->append("survivor");
    // dades dinàmiques
    stGameBoxDataValues = new QTextEdit();
    stGameBoxDataValues->setFocusPolicy(Qt::NoFocus);
    stGameBoxDataValues->setStyleSheet("background-color: black; color: white;");
    stGameBoxDataValues->setReadOnly(true);
    stGameBoxDataValues->append("0");   // iteration
    stGameBoxDataValues->append("0");   // life
    stGameBoxDataValues->append("0");   // in
    stGameBoxDataValues->append("0");   // cloned
    stGameBoxDataValues->append("0");   // out
    stGameBoxDataValues->append("none");// survivor

    stGameBoxDataTotals = new QTextEdit();
    stGameBoxDataTotals->setFocusPolicy(Qt::NoFocus);
    stGameBoxDataTotals->setStyleSheet("background-color: black; color: white;");
    stGameBoxDataTotals->setReadOnly(true);
    stGameBoxDataTotals->append("game");  // no data
    stGameBoxDataTotals->append("totals");  // header
    stGameBoxDataTotals->append("0");       // in
    stGameBoxDataTotals->append("0");       // cloned
    stGameBoxDataTotals->append("0");       // out
    // afegim els blocs de text al seu contenidor
    stGameBoxData->addWidget(stGameBoxDataLabels);
    stGameBoxData->addWidget(stGameBoxDataValues);
    stGameBoxData->addWidget(stGameBoxDataTotals);

    // afegim el contenidor de dades al contenidor de dades del joc
    stGameBox->addWidget(stGameBoxDataWidget);

    // contenidor per a les dades dels jugadors 'poma'
    //------------------------------------------------
    QWidget* stApplesBoxDataWidget = new QWidget();
    // organitzador horitzontal per a les dades
    QHBoxLayout* stApplesBoxData = new QHBoxLayout(stApplesBoxDataWidget);
    // dades a mostrar
    QTextEdit* stApplesBoxDataLabels = new QTextEdit();
    stApplesBoxDataLabels->setStyleSheet("background-color: black; color: white;");
    // etiquetes
    stApplesBoxDataLabels->setReadOnly(true);
    stApplesBoxDataLabels->append("in field");
    stApplesBoxDataLabels->append("grow");
    stApplesBoxDataLabels->append("eaten");
    // dades dinàmiques
    stApplesBoxDataValues = new QTextEdit();
    stApplesBoxDataValues->setFocusPolicy(Qt::NoFocus);
    stApplesBoxDataValues->setStyleSheet("background-color: black; color: white;");
    stApplesBoxDataValues->setReadOnly(true);
    stApplesBoxDataValues->append("0"); // life
    stApplesBoxDataValues->append("0"); // in
    stApplesBoxDataValues->append("0"); // out

    stApplesBoxDataTotals = new QTextEdit();
    stApplesBoxDataTotals->setFocusPolicy(Qt::NoFocus);
    stApplesBoxDataTotals->setStyleSheet("background-color: black; color: white;");
    stApplesBoxDataTotals->setReadOnly(true);
    stApplesBoxDataTotals->append("totals");    // header
    stApplesBoxDataTotals->append("0");         // in
    stApplesBoxDataTotals->append("0");         // out

    // afegim els blocs de text al seu contenidor
    stApplesBoxData->addWidget(stApplesBoxDataLabels);
    stApplesBoxData->addWidget(stApplesBoxDataValues);
    stApplesBoxData->addWidget(stApplesBoxDataTotals);
    // afegim el contenidor de dades
    stApplesBox->addWidget(stApplesBoxDataWidget);

    // contenidor per a les dades dels jugadors 'bé'
    //----------------------------------------------
    QWidget* stSheepsBoxDataWidget = new QWidget();
    // organitzador horitzontal per a les dades
    QHBoxLayout* stSheepsBoxData = new QHBoxLayout(stSheepsBoxDataWidget);
    // dades a mostrar
    // etiquetes
    QTextEdit* stSheepsBoxDataLabels = new QTextEdit();
    stSheepsBoxDataLabels->setStyleSheet("background-color: black; color: white;");
    stSheepsBoxDataLabels->setReadOnly(true);

    stSheepsBoxDataLabels->append("live");      // live
    stSheepsBoxDataLabels->append("born");      // in
    stSheepsBoxDataLabels->append("cloned");    // cloned
    stSheepsBoxDataLabels->append("dead");      // out
    stSheepsBoxDataLabels->append("evolution"); // generation
    // dades dinàmiques
    stSheepsBoxDataValues = new QTextEdit();
    stSheepsBoxDataValues->setFocusPolicy(Qt::NoFocus);
    stSheepsBoxDataValues->setStyleSheet("background-color: black; color: white;");
    stSheepsBoxDataValues->setReadOnly(true);
    stSheepsBoxDataValues->append("0"); // live
    stSheepsBoxDataValues->append("0"); // in
    stSheepsBoxDataValues->append("0"); // cloned
    stSheepsBoxDataValues->append("0"); // out
    stSheepsBoxDataValues->append("0"); // generation

    stSheepsBoxDataTotals = new QTextEdit();
    stSheepsBoxDataTotals->setFocusPolicy(Qt::NoFocus);
    stSheepsBoxDataTotals->setStyleSheet("background-color: black; color: white;");
    stSheepsBoxDataTotals->setReadOnly(true);
    stSheepsBoxDataTotals->append("totals");    // header
    stSheepsBoxDataTotals->append("0");         // in
    stSheepsBoxDataTotals->append("0");         // cloned
    stSheepsBoxDataTotals->append("0");         // out
    stSheepsBoxDataTotals->append("0");         // generation

    // afegim els blocs de text al seu contenidor
    stSheepsBoxData->addWidget(stSheepsBoxDataLabels);
    stSheepsBoxData->addWidget(stSheepsBoxDataValues);
    stSheepsBoxData->addWidget(stSheepsBoxDataTotals);
    // afegim el contenidor de dades
    stSheepsBox->addWidget(stSheepsBoxDataWidget);

    // contenidor per a les dades dels jugadors 'llop'
    //-----------------------------------------------
    QWidget* stWolvesBoxDataWidget = new QWidget();
    // organitzador horitzontal per a les dades
    QHBoxLayout* stWolvesBoxData = new QHBoxLayout(stWolvesBoxDataWidget);
    // dades a mostrar
    QTextEdit* stWolvesBoxDataLabels = new QTextEdit();
    // etiquetes
    stWolvesBoxDataLabels->setStyleSheet("background-color: black; color: white;");
    stWolvesBoxDataLabels->setReadOnly(true);
    stWolvesBoxDataLabels->append("live");      // live
    stWolvesBoxDataLabels->append("born");      // in
    stWolvesBoxDataLabels->append("cloned");    // cloned
    stWolvesBoxDataLabels->append("dead");      // out
    stWolvesBoxDataLabels->append("evolution"); // generation
    // dades dinàmiques
    stWolvesBoxDataValues = new QTextEdit();
    stWolvesBoxDataValues->setFocusPolicy(Qt::NoFocus);
    stWolvesBoxDataValues->setStyleSheet("background-color: black; color: white;");
    stWolvesBoxDataValues->setReadOnly(true);
    stWolvesBoxDataValues->append("0"); // live
    stWolvesBoxDataValues->append("0"); // in
    stWolvesBoxDataValues->append("0"); // cloned
    stWolvesBoxDataValues->append("0"); // out
    stWolvesBoxDataValues->append("0"); // generation

    stWolvesBoxDataTotals = new QTextEdit();
    stWolvesBoxDataTotals->setFocusPolicy(Qt::NoFocus);
    stWolvesBoxDataTotals->setStyleSheet("background-color: black; color: white;");
    stWolvesBoxDataTotals->setReadOnly(true);
    stWolvesBoxDataTotals->append("totals");    // header
    stWolvesBoxDataTotals->append("0");         // in
    stWolvesBoxDataTotals->append("0");         // cloned
    stWolvesBoxDataTotals->append("0");         // out
    stWolvesBoxDataTotals->append("0");         // generation
    // afegim els blocs de text al seu contenidor
    stWolvesBoxData->addWidget(stWolvesBoxDataLabels);
    stWolvesBoxData->addWidget(stWolvesBoxDataValues);
    stWolvesBoxData->addWidget(stWolvesBoxDataTotals);
    // afegim el contenidor de dades
    stWolvesBox->addWidget(stWolvesBoxDataWidget);
}
