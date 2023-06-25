#include "global.h"
#include "mygameview.h"

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent)
{
}

GameView::GameView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    // mida fixe i sense barres de scroll
    this->setFixedSize(scene->width(),scene->height());
    this->setFrameStyle(0);
}

void GameView::onKeyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key_Space:
            { qDebug() << "Space"; break; }
        case Qt::Key_Left:
            { qDebug() << "Left"; break; }
        case Qt::Key_Right:
            { qDebug() << "Right"; break; }
        default:
            { qDebug() << "Unhandled"; break; }
    }
}
