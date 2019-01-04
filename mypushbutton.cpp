#include "mypushbutton.h"
#include "QDebug"

MyPushButton::MyPushButton(QWidget *parent):QPushButton(parent)
{
    this->setStyleSheet("QPushButton{border-image:url(:/Image/min.png)} "
                        "QPushButton:hover{background-color:#00BFFF;}"
                        "QPushButton:pressed{background-color:#009ACD;}");
}

void MyPushButton::focusOutEvent(QFocusEvent *e)
{
    qDebug()<<"MyPushButton::focusOutEvent";
}

MyPushButton::~MyPushButton()
{

}
