#ifndef MYSTATSDIALOG_H
#define MYSTATSDIALOG_H

#include "global.h"

class MyStatsDialog : public QDialog
{
private:

    Q_OBJECT

public:

QTextEdit* stGameBoxDataValues=nullptr;
QTextEdit* stGameBoxDataTotals=nullptr;
QTextEdit* stApplesBoxDataValues=nullptr;
QTextEdit* stApplesBoxDataTotals=nullptr;
QTextEdit* stSheepsBoxDataValues=nullptr;
QTextEdit* stSheepsBoxDataTotals=nullptr;
QTextEdit* stWolvesBoxDataValues=nullptr;
QTextEdit* stWolvesBoxDataTotals=nullptr;

explicit MyStatsDialog(QWidget *parent = nullptr);
    void dInit();

signals:

};

#endif // MYSTATSDIALOG_H

