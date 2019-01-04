#ifndef INCOMINGMSGITEM_H
#define INCOMINGMSGITEM_H

#include <QWidget>
#include "QPixmap"
#include "QLabel"
#include "QPushButton"

class IncomingMsgItem : public QWidget
{
    Q_OBJECT
public:
    explicit IncomingMsgItem(QWidget *parent,
                             bool isGroupInd,
                             QString nickNameString,
                             QString lastMsgDateTime,
                             QString lastMsgFromUserName,
                             QString lastMsgDetail,
                             QString iconPath,
                             int unreadMsgCnt,
                             QString miniIconPath,
                             int maxWidth);
    QPixmap getRoundImage(const QPixmap &src);
//    bool eventFilter(QObject *watched, QEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void setNickName(const QString &name);
    void setUserIcon(const QString &path);
    void setLastMsg(const QString &msg);
    void setLastMsgDatetime(const QString &datetime);
    void setUnreadMsgCnt(const int count);
    //void getInfoFromDataBase();
    void init();
    QPixmap drawUnreadMsgPic(int msgCount);
    void setMiniIconPic(QPixmap src);
signals:

public slots:

private:
    int index;
    QLabel *icon;
    QLabel *nickName;
    QLabel *latestMsg;
    QLabel *timeLabel;
    QString iconFilePath;
    QString nickNameString;
    QString defaultIconFilePath = ":/Image/QQ.jpg";
    bool isMousePressed = false;
    bool isGroup = false;
    int msgCount = 0;
    QLabel *miniIcon;
};

#endif // INCOMINGMSGITEM_H
