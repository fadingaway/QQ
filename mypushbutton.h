#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include "QPushButton"
class MyPushButton: public QPushButton
{
    Q_OBJECT
    explicit MyPushButton(QWidget *parent = 0);
    ~MyPushButton();

    void focusOutEvent(QFocusEvent *e);
};

#endif // MYPUSHBUTTON_H
