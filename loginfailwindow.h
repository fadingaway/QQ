#ifndef LOGINFAILWINDOW_H
#define LOGINFAILWINDOW_H

#include <QWidget>
#include "QPushButton"
#include "QLabel"
#include "QPoint"
#include "QLineEdit"
#include "QComboBox"

class LoginFailWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginFailWindow(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void onButtonClicked();
signals:
    void returnToMainWindow();
private:
    QPushButton *buttonMin;
    QPushButton *buttonClose;
    bool isMousePressed = false;
    QPoint MousePressPos;

    QWidget *mainWidget;
    QLabel *titleBar;
    QLabel *content;
    QLabel *bottom;
};

#endif // LOGINFAILWINDOW_H
