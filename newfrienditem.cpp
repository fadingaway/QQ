#include "newfrienditem.h"
#include "QVBoxLayout"
#include "QLabel"
#include "QPushButton"
#include "QPixmap"
#include "QPainter"
#include "QDebug"
#include "QEvent"
newFriendItem::newFriendItem(QWidget *parent,
                             QString userAcctId,
                             QString userIconPath,
                             QString userName,
                             int totalCommenFriends) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(75,105);
    //this->setStyleSheet("border:1px solid black;");
    this->userAcctId = userAcctId;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    userIconLabel = new QLabel(this);
    userIconLabel->setFixedSize(75,40);
    QPixmap icon(userIconPath);
    icon = getRoundImage(icon);
    userIconLabel->setPixmap(icon);
    userIconLabel->setAlignment(Qt::AlignCenter);
    userIconLabel->setContentsMargins(0,0,0,0);

    userNameLabel = new QLabel(this);
    userName = getElidedText(userNameLabel->font(),userName, 70);
    userNameLabel->setText(userName);
    userNameLabel->setAlignment(Qt::AlignCenter);
    userNameLabel->setStyleSheet("font-family:微软雅黑;font-size:8pt;color:black;");
    userNameLabel->setContentsMargins(0,0,0,0);

    text = new QLabel(this);
    text->setText(QString("%1位共同好友").arg(totalCommenFriends));
    text->setStyleSheet("font-family:微软雅黑;font-size:8pt;color:gray;");
    text->setContentsMargins(0,0,0,0);
    text->setAlignment(Qt::AlignCenter);

    addFriend = new QPushButton(this);
    addFriend->setFlat(true);
    addFriend->setText("添加");
    addFriend->setFixedSize(55,27);
    addFriend->setStyleSheet("QPushButton{font-family:微软雅黑;font-size:9pt;color:black;"
                             "border:1px solid lightgray;"
                             "background-color:white;"
                             "border-radius:3px;}"
                             "QPushButton::hover{background-color:rgb(97,132,202);}");
    connect(addFriend, &QPushButton::clicked, this, &newFriendItem::addNewFriend);
    mainLayout->addWidget(userIconLabel, Qt::AlignCenter);
    mainLayout->addWidget(userNameLabel);
    mainLayout->addWidget(text);
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(addFriend, Qt::AlignCenter);
    buttonLayout->setContentsMargins(10,0,0,0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    deleteItemButton = new QPushButton(this);
    deleteItemButton->setFlat(true);
    deleteItemButton->setFixedSize(25,25);
    deleteItemButton->setFocusPolicy(Qt::NoFocus);
    deleteItemButton->setCursor(Qt::PointingHandCursor);
    deleteItemButton->setAttribute(Qt::WA_TranslucentBackground);
    deleteItemButton->setStyleSheet("background-image:url(:/Image/delete.png);"
                              "border-style:none;");
    connect(deleteItemButton, &QPushButton::clicked, this, &newFriendItem::deleteItem);
    deleteItemButton->move(50,0);
    deleteItemButton->hide();

    this->installEventFilter(this);
    this->setMouseTracking(true);
}

QPixmap newFriendItem::getRoundImage(const QPixmap &src)
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

void newFriendItem::addNewFriend()
{
    emit requestToAddFriend(userAcctId);
    this->close();
}

QString newFriendItem::getElidedText(QFont font, QString str, int MaxWidth)
{
    QFontMetrics fontWidth(font);
    int width = fontWidth.width(str);  //计算字符串宽度

    if(width >= MaxWidth)  //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str, Qt::ElideRight, MaxWidth);  //右部显示省略号
    }
    return str;   //返回处理后的字符串
}

QString newFriendItem::getUserAcctId()
{
    return userAcctId;
}

void newFriendItem::deleteItem()
{
    emit requestToDeleteItem(this->userAcctId);
}

void newFriendItem::setDelButtonVisible(bool visible)
{
    deleteItemButton->setHidden(!visible);
}

bool newFriendItem::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this)
    {
        if(event->type() == QEvent::Enter)
        {
            deleteItemButton->show();
        }
        else if(event->type() == QEvent::Leave)
        {
            deleteItemButton->hide();
        }
    }
    return QWidget::eventFilter(watched, event);
}
