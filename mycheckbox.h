#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>

class SelectorDialog;

class MyCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    MyCheckBox(int x, int y);
    int x;
    int y;
    SelectorDialog* parentClass;


public slots:
    void onStateChanged2(int state);

};

#endif // MYCHECKBOX_H

