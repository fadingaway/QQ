#include "mytextedit.h"
#include "QKeyEvent"
#include "QDebug"

MyTextEdit::MyTextEdit(QWidget *parent):QTextEdit(parent)
{

}

void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter
            ||event->key() == Qt::Key_Return)
    {
        emit enterKeyPressed();
    }
    else
    {
        return QTextEdit::keyPressEvent(event);
    }
}
