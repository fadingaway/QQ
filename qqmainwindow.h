#ifndef QQMAINWINDOW_H
#define QQMAINWINDOW_H

#include <QWidget>
#include "QLabel"
#include "QTabBar"
#include "QTabWidget"
#include "QPixmap"

class QQMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QQMainWindow(QWidget *parent = 0);
    void readSetting();
    void initTitle();
    QPixmap getRoundImage(const QPixmap &src, int radius);
signals:

public slots:

private:
    QLabel *headerLabel;
    QLabel *titleLabel;
    QLabel *userinfoLabel;
    QLabel *weatherLabel;
    QLabel *searchLabel;

    QTabBar *tabBar;
    QTabWidget *msgTab;
    QTabWidget *contactTab;
    QTabWidget *newsTab;

    QLabel *bottomLabel;

    QPixmap userHeadPic;
    QPixmap defaultUserHeadPic;
    QString nickName;
    QString signatureText;
};

#endif // QQMAINWINDOW_H
