#ifndef MYGAMEVIEW_H
#define MYGAMEVIEW_H

#include "global.h"

class GameView : public QGraphicsView
{
private:

    Q_OBJECT

public:
    explicit GameView(QWidget *parent = nullptr);
    GameView(QGraphicsScene *scene, QWidget *parent = nullptr);

Q_SIGNALS:
      void keyPressEvent(QKeyEvent* event);

public slots:
      void onKeyPressEvent (QKeyEvent*);
};

#endif
