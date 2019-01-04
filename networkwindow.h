#ifndef NETWORKWINDOW_H
#define NETWORKWINDOW_H

#include <QObject>
#include <QWidget>
#include "QPushButton"
#include "QLabel"
#include "QPoint"
#include "QLineEdit"
#include "QComboBox"

class NetworkWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkWindow(QWidget *parent = 0);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void updateWidgetStatus(const QString &text);
    void updateTestButtonStatus();
    void networkTest();
    void returnToMainWindow();
    void paintEvent(QPaintEvent *event);
    void startToShow();
public slots:
    void onRotateFinished();
signals:
    void rotateFinished(QPoint point);
private:
    QPushButton *buttonMin;
    QPushButton *buttonClose;
    bool isMousePressed = false;
    QPoint MousePressPos;
    QLabel *titleBar;

    QLabel *content;
    QLineEdit *editAddress;
    QLineEdit *editPort;
    QLineEdit *editUserName;
    QLineEdit *editPassword;
    QLineEdit *editRegion;
    QPushButton *buttonTest;
    QComboBox *comboLoginAddress;
    QLineEdit *editLoginPort;
    QLabel *bottom;
    bool isRotating = false;
    QPixmap rotatePixmap;

    QLabel *mainWidget;
    bool isToShow  = false;
};

#endif // NETWORKWINDOW_H
