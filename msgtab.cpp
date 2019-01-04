#include "msgtab.h"
#include "incomingmsgitem.h"
#include "QListWidgetItem"
#include "QVBoxLayout"
#include "QDebug"
MsgTab::MsgTab(QWidget *parent, int width, int height) : QWidget(parent)
{
    width = 370;
    height = 600;
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(width, height);
    listView = new QListWidget(this);
    listView->setFixedWidth(width-10);
    getInfoFromDatabase();

    for(int i = 0; i<msgItems.size(); i++)
    {
        IncomingMsgItem *incomingmsgitem
                = new IncomingMsgItem(listView,
                                      msgItems.at(i).isGroupInd,
                                      msgItems.at(i).nickNameString,
                                      msgItems.at(i).lastMsgDateTime,
                                      msgItems.at(i).lastMsgFromUserName,
                                      msgItems.at(i).lastMsgDetail,
                                      msgItems.at(i).iconPath,
                                      msgItems.at(i).unreadMsgCnt,
                                      msgItems.at(i).miniIconPath,
                                      width-10);
        QListWidgetItem *listItem = new QListWidgetItem(listView);
        listItem->setSizeHint(incomingmsgitem->sizeHint());
        listView->setItemWidget(listItem, incomingmsgitem);
    }
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setMouseTracking(true);
    listView->setFocusPolicy(Qt::NoFocus);
    listView->setStyleSheet("QListWidget{border-radius:5px;background-color: rgba(255,255,255,240);}"
                            "QListWidget::item{margin:6px 0 3px 0;}\
                             QListWidget::item:selected{border-radius:3px;\
                                                        background-color:rgba(106 , 171 , 219 , 80);}\
                             QListWidget::item:hover{border-radius:3px;\
                                                        background-color:rgb(220 , 220 , 220);}");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(listView);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
    this->setContentsMargins(0,0,0,0);
}

void MsgTab::getInfoFromDatabase()
{
    msgItem item;

    item.iconPath = QString(":/Image/head_1.jpg");
    item.isGroupInd = true;
    item.lastMsgDateTime = QString("20:22");
    item.lastMsgFromUserName = QString("小前先生");
    item.lastMsgDetail = QString("@Geek-毒蘑菇 借钱");
    item.nickNameString = QString("Qt编程爱好者");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_1.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_2.jpg");
    item.isGroupInd = true;
    item.lastMsgDateTime = QString("20:22");
    item.lastMsgFromUserName = QString("死骑的前世");
    item.lastMsgDetail = QString("这么小睫毛就这么长哦");
    item.nickNameString = QString("天涯纱布公会群");
    item.unreadMsgCnt = 18;
    item.miniIconPath = QString(":/Image/head_2.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_3.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("20:22");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("[图片]");
    item.nickNameString = QString("我的Android手机");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_3.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_4.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("20:16");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("上下床+榻榻米=家变大20m^2");
    item.nickNameString = QString("QQ精选");
    item.unreadMsgCnt = 1;
    item.miniIconPath = QString(":/Image/head_4.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_5.jpg");
    item.isGroupInd = true;
    item.lastMsgDateTime = QString("9:11");
    item.lastMsgFromUserName = QString("Lucifer");
    item.lastMsgDetail = QString("[图片]");
    item.nickNameString = QString("影视+剧集+音乐+生活");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_5.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_6.jpg");
    item.isGroupInd = true;
    item.lastMsgDateTime = QString("10-9");
    item.lastMsgFromUserName = QString("一切随风");
    item.lastMsgDetail = QString("。。。。。");
    item.nickNameString = QString("菊花台");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_6.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_1.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("10-8");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("");
    item.nickNameString = QString("赵晓东");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_1.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_2.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("10-8");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("转弯");
    item.nickNameString = QString("深海&海豚");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_2.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_3.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("10-7");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("柳顺国上传了视频");
    item.nickNameString = QString("好友动态");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_3.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_4.jpg");
    item.isGroupInd = true;
    item.lastMsgDateTime = QString("10-3");
    item.lastMsgFromUserName = QString("Android Studio M群");
    item.lastMsgDetail = QString("okhttp如何持久化session1");
    item.nickNameString = QString("群助手");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_4.jpg");
    msgItems.append(item);

    item.iconPath = QString(":/Image/head_5.jpg");
    item.isGroupInd = false;
    item.lastMsgDateTime = QString("9-30");
    item.lastMsgFromUserName = QString("");
    item.lastMsgDetail = QString("");
    item.nickNameString = QString("毕晓兰");
    item.unreadMsgCnt = 0;
    item.miniIconPath = QString(":/Image/head_5.jpg");
    msgItems.append(item);
}

void MsgTab::onItemDoubleClicked()
{
    qDebug()<<"onItemDoubleClicked()";
}

void MsgTab::onItemRightClicked()
{
    qDebug()<<"onItemRightClicked()";
}

