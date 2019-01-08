#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H
#include "QWidget"
#include "QTextEdit"

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget *parent);
    void keyPressEvent(QKeyEvent *event);
signals:
    void enterKeyPressed();
};

#endif // MYTEXTEDIT_H
