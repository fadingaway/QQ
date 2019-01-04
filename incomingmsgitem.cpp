#include "incomingmsgitem.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QPixmap"
#include "QPushButton"
#include "QPainter"
#include "QListWidgetItem"
#include "QMouseEvent"
#include "QDebug"
#include "QTime"
#include "QLabel"
#include "QSqlDatabase"
#include "QSqlQuery"

IncomingMsgItem::IncomingMsgItem(QWidget *parent,
                                 bool isGroupInd,
                                 QString nickNameString,
                                 QString lastMsgDateTime,
                                 QString lastMsgFromUserName,
                                 QString lastMsgDetail,
                                 QString iconPath,
                                 int unreadMsgCnt,
                                 QString miniIconPath,
                                 int maxWidth)
    :QWidget(parent)
{
    isGroup = isGroupInd;
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("font-family:微软雅黑;");
    this->setFixedSize(maxWidth,70);
    //getInfoFromDataBase();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    icon = new QLabel;
    icon->setContentsMargins(5,5,5,5);

    QVBoxLayout *contentLayout = new QVBoxLayout;
    nickName = new QLabel;
    nickName->setFont(QFont("微软雅黑",10));
    nickName->setContentsMargins(0,0,0,0);

    latestMsg = new QLabel;
    latestMsg->setFont(QFont("微软雅黑",9));
    latestMsg->setStyleSheet("color:Gray;");
    latestMsg->setContentsMargins(0,0,0,0);
    contentLayout->addWidget(nickName,1);
    contentLayout->addWidget(latestMsg,1);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(10,0,0,0);

    QVBoxLayout *timeAndMsgCountLayout = new QVBoxLayout;
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("font-family:微软雅黑;font-size:9pt;color:gray;");
    timeLabel->setContentsMargins(0,0,5,0);

    QLabel *msgCount = new QLabel(this);
    msgCount->setPixmap(drawUnreadMsgPic(unreadMsgCnt));
    QSizePolicy sizepolicy = msgCount->sizePolicy();
    sizepolicy.setRetainSizeWhenHidden(true);
    msgCount->setSizePolicy(sizepolicy);
    msgCount->setContentsMargins(10,0,5,0);

    timeAndMsgCountLayout->addWidget(timeLabel,1);
    timeAndMsgCountLayout->addWidget(msgCount,1);
    timeAndMsgCountLayout->setSpacing(0);
    timeAndMsgCountLayout->setContentsMargins(5,0,0,0);

    mainLayout->addWidget(icon, 1, Qt::AlignLeading);
    mainLayout->addLayout(contentLayout, 7);
    mainLayout->addLayout(timeAndMsgCountLayout, 1);
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
    this->setMouseTracking(true);

    miniIcon = new QLabel(icon);
    miniIcon->setFixedSize(14,14);
    miniIcon->move(31, 31);
    miniIcon->setAttribute(Qt::WA_TranslucentBackground);
    miniIcon->show();

    setNickName(nickNameString);
    setUserIcon(iconPath);
    if(!lastMsgFromUserName.isEmpty()&&!lastMsgDetail.isEmpty())
    {
        setLastMsg(QString(lastMsgFromUserName+":"+lastMsgDetail));
    }
    else if(lastMsgFromUserName.isEmpty()&&!lastMsgDetail.isEmpty())
    {
        setLastMsg(lastMsgDetail);
    }
    else
    {
        setLastMsg("");
    }
    setLastMsgDatetime(lastMsgDateTime);
    setUnreadMsgCnt(unreadMsgCnt);

    QPixmap pixmap(miniIconPath);
    setMiniIconPic(pixmap);
}

QPixmap IncomingMsgItem::getRoundImage(const QPixmap &src)
{
    int radius = 18;
    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    return pixmap;
}

//bool IncomingMsgItem::eventFilter(QObject *watched, QEvent *event)
//{
//    if(watched == nickName)
//    {
//        qDebug()<<"eventFilter";
//        if(event->type() == QEvent::HoverEnter)
//        {
//            qDebug()<<"HoverEnter";
//            nickName->setStyleSheet("background-color:gray;");
//        }
//        else if(event->type() == QEvent::HoverLeave)
//        {
//            qDebug()<<"HoverLeave";
//            nickName->setStyleSheet("background-color:white;");
//        }
//        event->accept();
//    }
//    return QWidget::eventFilter(watched, event);
//}

//void IncomingMsgItem::mousePressEvent(QMouseEvent *event)
//{
//    if(event->button() == Qt::LeftButton)
//        isMousePressed = true;
//}

//void IncomingMsgItem::mouseReleaseEvent(QMouseEvent *event)
//{
//    if(isMousePressed)
//    isMousePressed = false;
//}

void IncomingMsgItem::setNickName(const QString &name)
{
    nickNameString = name;
    nickName->setText(nickNameString);
}

void IncomingMsgItem::setUserIcon(const QString &path)
{
    iconFilePath = path;
    QPixmap pixmap(iconFilePath);
    pixmap = getRoundImage(pixmap);
    icon->setPixmap(pixmap);
}

void IncomingMsgItem::setLastMsg(const QString &msg)
{
    latestMsg->setText(msg);
}

void IncomingMsgItem::setLastMsgDatetime(const QString &datetime)
{
    timeLabel->setText(datetime);
}

void IncomingMsgItem::setUnreadMsgCnt(const int count)
{
    msgCount = count;
}

QPixmap IncomingMsgItem::drawUnreadMsgPic(int msgCount)
{
    QPixmap pixmap(QSize(30,25));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    if(msgCount == 0)
    {
        painter.end();
        return pixmap;
    }
    else if(msgCount <10)
    {
        if(isGroup)
        {
            painter.setPen(QColor(140,211,239));
            painter.setBrush(QColor(140,211,239));
            painter.drawEllipse(QPoint(11,11),8,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,4,14,14),QString::number(msgCount), option);
            painter.end();
            return pixmap;
        }
        else
        {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(11,11),8,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,4,14,14),QString::number(msgCount),option);
            painter.end();
            return pixmap;
        }
    }
    else if(msgCount<100)
    {
        if(isGroup)
        {
            painter.setPen(QColor(140,211,239));
            painter.setBrush(QColor(140,211,239));
            painter.drawEllipse(QPoint(11,11),9,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,2,14,14),QString::number(msgCount),option);
            painter.end();
            return pixmap;
        }
        else
        {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(11,11),9,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,2,14,14),QString::number(msgCount),option);
            painter.end();
            return pixmap;
        }
    }
    else if(msgCount>99)
    {
        if(isGroup)
        {
            painter.setPen(QColor(140,211,239));
            painter.setBrush(QColor(140,211,239));
            painter.drawEllipse(QPoint(13,12),12,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,1,20,20),QString("99+"),option);
            painter.end();
            return pixmap;
        }
        else
        {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(13,12),12,8);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4,1,20,20),QString("99+"),option);
            painter.end();
            return pixmap;
        }
    }
}

void IncomingMsgItem::setMiniIconPic(QPixmap src)
{
    miniIcon->setPixmap(src.scaled(14,14));
}
//void IncomingMsgItem::getInfoFromDataBase()
//{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("qq");
//    if(!db.open())
//    {
//        qDebug()<<"Cannot Open Database!";
//        return;
//    }
//    QSqlQuery query;

//}























