#include "contactitem.h"
#include "QPixmap"
#include "QPainter"
#include "QSize"
#include "QVBoxLayout"
#include "QDebug"
#include "QImage"
#include "QImageReader"
#include "QMouseEvent"
#include "QGraphicsDropShadowEffect"
ContactItem::ContactItem(QWidget *parent,
                         int width,
                         QString userIconPath,
                         QString userName,
                         int userStatus,
                         QString userSignature,
                         int layout_mode) : QWidget(parent)
{
    if(layout_mode == 1)
    {
        height = 45;
        this->height = height;
    }
    else if(layout_mode == 2)
    {
        height = 55;
        this->height = height;
    }
    this->userIconPath = userIconPath;
    this->userName = userName;
    this->userStatus = userStatus;
    this->userSignature = userSignature;
    this->layout_mode = layout_mode;

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(width,height);
    //this->setStyleSheet("border:1px solid gray;");

    userIcon = new QLabel;
    userIcon->setFixedSize(height-10,height-10);
    QPixmap pixmap(userIconPath);
    pixmap = getRoundImage(pixmap, userIcon->width()/2-4);
    userIcon->setPixmap(pixmap);
    userIcon->setAttribute(Qt::WA_TranslucentBackground);

    userNameLabel = new QLabel;
    userNameLabel->setText(userName);
    userNameLabel->setContentsMargins(5,0,0,5);
    userNameLabel->setStyleSheet("font-family:微软雅黑;font-size:10pt;color:black;");
    userNameLabel->setFixedHeight(height-10);
    userNameLabel->setAlignment(Qt::AlignBottom);
    userNameLabel->adjustSize();
    userNameLabel->setAttribute(Qt::WA_TranslucentBackground);

    userSignatureLabel = new QLabel;
    userSignatureLabel->setStyleSheet("font-family:微软雅黑;font-size:8pt;color:gray;");
    userSignatureLabel->setFixedHeight(height-10);
    userSignatureLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
    userSignatureLabel->setContentsMargins(0,0,0,5);
    userSignatureLabel->setText(userSignature);
    userSignatureLabel->adjustSize();
    userSignatureLabel->setAttribute(Qt::WA_TranslucentBackground);

    mainLayout_mode_1 = new QHBoxLayout;
    mainLayout_mode_2 = new QHBoxLayout;

    miniStatusIcon = new QLabel(this);
    miniStatusIcon->setAttribute(Qt::WA_TranslucentBackground);

    if(layout_mode == 2)
    {
        userNameLabel->setFixedHeight((height-10)/2);
        userNameLabel->setAlignment(Qt::AlignLeft);
        userNameLabel->setContentsMargins(5,5,0,0);

        userSignatureLabel->setFixedHeight((height-10)/2);
        userSignatureLabel->setAlignment(Qt::AlignLeft);
        userSignatureLabel->setContentsMargins(5,3,0,0);

        QVBoxLayout *contentLayout = new QVBoxLayout;
        contentLayout->addWidget(userNameLabel);
        contentLayout->addWidget(userSignatureLabel);
        contentLayout->setContentsMargins(0,0,0,0);
        mainLayout_mode_2->addWidget(userIcon, 1, Qt::AlignLeading);
        mainLayout_mode_2->addLayout(contentLayout);
        mainLayout_mode_2->setSpacing(0);
        mainLayout_mode_2->setContentsMargins(10,5,5,5);

        calculateSignatureLengh(layout_mode);
        this->setLayout(mainLayout_mode_2);

        miniStatusIcon->setFixedSize(userIcon->width()/3,userIcon->width()/3);
        miniStatusIcon->move(userIcon->width()*3/4, userIcon->height()*3/4);
        setMiniStatusIcon(userStatus);
        miniStatusIcon->raise();
    }
    else
    {
        mainLayout_mode_1->addWidget(userIcon,1, Qt::AlignLeading);
        mainLayout_mode_1->addWidget(userNameLabel,1,  Qt::AlignLeft|Qt::AlignBottom);
        mainLayout_mode_1->addWidget(userSignatureLabel,6, Qt::AlignLeft|Qt::AlignBottom);
        mainLayout_mode_1->setSpacing(5);
        mainLayout_mode_1->setContentsMargins(10,5,5,5);

        calculateSignatureLengh(layout_mode);
        this->setLayout(mainLayout_mode_1);

        miniStatusIcon->setFixedSize(userIcon->width()/3,userIcon->width()/3);
        miniStatusIcon->move(userIcon->x()+ userIcon->width()*3/4, userIcon->y() + userIcon->height()*3/4);
        setMiniStatusIcon(userStatus);
        miniStatusIcon->raise();
    }
}
QPixmap ContactItem::getRoundImage(const QPixmap &src, int radius)
{
    QSize size(2*radius+4, 2*radius+4);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addEllipse(0, 0, 2*radius+4, 2*radius+4);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 2*radius+4, 2*radius+4, src);

    return pixmap;
}

void ContactItem::setMiniStatusIcon(int status)
{
    switch(status)
    {
        case LOGIN_BY_MOBILE:
            miniStatusIcon->setPixmap(QPixmap(":/Image/online.png").scaled(miniStatusIcon->size()));
            miniStatusIcon->show();
            break;
        case LOGIN_BY_PC:
            miniStatusIcon->setPixmap(QPixmap(":/Image/busy.png").scaled(miniStatusIcon->size()));
            miniStatusIcon->show();
            break;
        case LOGIN_BY_IPHONE:
            miniStatusIcon->setPixmap(QPixmap(":/Image/Nodisturb.png").scaled(miniStatusIcon->size()));
            miniStatusIcon->show();
            break;
        default:
            miniStatusIcon->setPixmap(QPixmap(""));
            miniStatusIcon->show();
            break;
    }
}

QString ContactItem::getElidedText(QFont font, QString str, int MaxWidth)
{
    QFontMetrics fontWidth(font);
    int width = fontWidth.width(str);  //计算字符串宽度

    if(width >= MaxWidth)  //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str, Qt::ElideRight, MaxWidth);  //右部显示省略号
    }
    return str;   //返回处理后的字符串
}

void ContactItem::refreshUserStatus(int userStatus)
{
    setMiniStatusIcon(userStatus);
    this->userStatus = userStatus;
}

void ContactItem::refreshUserName(QString userName)
{
    userNameLabel->setText(userName);
    this->userName = userName;
}

void ContactItem::refreshUserSignature(QString userSignature)
{
    userSignatureLabel->setText(userSignature);
    if(userSignatureLabel->width() >= (this->width()
                                       - userIcon->width()
                                       - userNameLabel->width()
                                       //下面要设置的content margins
                                       - 25
                                       //下面要设置的space = 5
                                       - 10
                                       ))
    {
        userSignature = userSignatureLabel->text();
        userSignature = getElidedText(userSignatureLabel->font(),
                                      userSignature,
                                      this->width()
                                      - userIcon->width()
                                      - userNameLabel->width()
                                      - 25
                                      - 10
                                      );
        userSignatureLabel->setText(userSignature);
    }
}

void ContactItem::refreshUserOnline(bool isOnline)
{
    if(isOnline)
    {
        QPixmap pixmap(userIconPath);
        pixmap.scaled(userIcon->size());
        userIcon->setPixmap(pixmap);
    }
    else
    {
        QImage image;
        image.load(userIconPath);
        image = toGray(image);
        QPixmap pixmap(image.size());
        pixmap.convertFromImage(image);
        pixmap = getRoundImage(pixmap,(height-10)/2);
        userIcon->setPixmap(pixmap);
    }
}

QImage ContactItem::toGray( QImage image )
{
    int height = image.height();
    int width = image.width();
    QImage ret(width, height, QImage::Format_Indexed8);
    ret.setColorCount(256);
    for(int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        for(int i = 0; i < height; i ++)
        {
            const uchar *pSrc = (uchar *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);
            memcpy(pDest, pSrc, width);
        }
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        for(int i = 0; i < height; i ++)
        {
            const QRgb *pSrc = (QRgb *)image.constScanLine(i);
            uchar *pDest = (uchar *)ret.scanLine(i);

            for( int j = 0; j < width; j ++)
            {
                 pDest[j] = qGray(pSrc[j]);
            }
        }
        break;
    default:
        break;
    }
    return ret;
}

void ContactItem::refreshUserIcon(QString userIconPath)
{
    this->userIconPath = userIconPath;
    userIcon->setPixmap(QPixmap(userIconPath));
}

void ContactItem::setFontSize(int fontSize)
{
    QString strUserName = QString("font-family:微软雅黑;font-size:%1pt;color:black;").arg(fontSize);
    QString strSignature = QString("font-family:微软雅黑;font-size:%1pt;color:gray;").arg(fontSize-2);
    userNameLabel->setStyleSheet(strUserName);
    userSignatureLabel->setStyleSheet(strSignature);
    calculateSignatureLengh(layout_mode);
}

void ContactItem::calculateSignatureLengh(int mode)
{
    QString tempSignature;
    if(mode == 1)
    {
        userSignatureLabel->setFixedWidth(this->width()
                                          - userIcon->width()
                                          - userNameLabel->width()
                                          //下面要设置的content margins
                                          - 15
                                          //下面要设置的space = 5
                                          - 10);
        tempSignature = getElidedText(userSignatureLabel->font(),
                                      userSignature,
                                      this->width()
                                      - userIcon->width()
                                      - userNameLabel->width()
                                      - 25
                                      - 16
                                      );
        userSignatureLabel->setText(tempSignature);
    }
    else
    {
        userSignatureLabel->setFixedWidth(this->width()
                                          - userIcon->width()
                                          //下面要设置的content margins
                                          - 10
                                          //下面要设置的space = 5
                                          - 5);
        userNameLabel->setFixedWidth(this->width()
                                          - userIcon->width()
                                          //下面要设置的content margins
                                          - 10
                                          //下面要设置的space = 5
                                          - 5);
        tempSignature = getElidedText(userSignatureLabel->font(),
                                      userSignature,
                                      this->width()
                                      - userIcon->width()
                                      - 15
                                      - 5
                                      );
        userSignatureLabel->setText(tempSignature);
    }
}

void ContactItem::setLayoutMode(int layoutMode)
{
    if(layoutMode == 2)
    {
        this->layout_mode = 2;
        this->height = 55;
        this->setFixedSize(this->width(),height);
        delete this->layout();

        userIcon->setFixedSize(height-10,height-10);
        QPixmap pixmap(userIconPath);
        pixmap = getRoundImage(pixmap, userIcon->width()/2-4);
        userIcon->setPixmap(pixmap);

        userNameLabel->setFixedHeight((height-10)/2);
        userNameLabel->setAlignment(Qt::AlignLeft);
        userNameLabel->setContentsMargins(0,5,0,0);
        userNameLabel->setText(userName);
        userNameLabel->adjustSize();

        userSignatureLabel->setFixedHeight((height-10)/2);
        userSignatureLabel->setAlignment(Qt::AlignLeft);
        userSignatureLabel->setContentsMargins(0,3,0,0);
        userSignatureLabel->adjustSize();
        calculateSignatureLengh(this->layout_mode);

        QVBoxLayout *contentLayout = new QVBoxLayout;
        contentLayout->addWidget(userNameLabel, Qt::AlignLeft);
        contentLayout->addWidget(userSignatureLabel, Qt::AlignLeft);
        contentLayout->setContentsMargins(0,0,0,0);
        contentLayout->setSpacing(0);
        contentLayout->setStretch(0,0);
        mainLayout_mode_2 = new QHBoxLayout;
        mainLayout_mode_2->addWidget(userIcon, 1, Qt::AlignLeading);
        mainLayout_mode_2->addLayout(contentLayout);
        mainLayout_mode_2->setSpacing(0);
        mainLayout_mode_2->setContentsMargins(10,5,5,5);
        this->setLayout(mainLayout_mode_2);

        miniStatusIcon->setFixedSize(userIcon->width()/3,userIcon->width()/3);
        miniStatusIcon->move(userIcon->width()*3/4, userIcon->height()*3/4);
        setMiniStatusIcon(userStatus);
        miniStatusIcon->raise();
    }
    else
    {
        this->height = 45;
        this->layout_mode = 1;
        this->setFixedSize(this->width(),height);
        this->setStyleSheet("background-color:transparent");
        delete this->layout();

        userIcon->setFixedSize(height-10,height-10);
        QPixmap pixmap(userIconPath);
        pixmap = getRoundImage(pixmap, userIcon->width()/2-4);
        userIcon->setPixmap(pixmap);

        userNameLabel->setFixedHeight(height-10);
        userNameLabel->setAlignment(Qt::AlignBottom);
        userNameLabel->setText(userName);
        QFontMetrics fontwidth(userNameLabel->font());
        int width = fontwidth.width(userName);
        userNameLabel->setFixedWidth(width+10);
        userNameLabel->setContentsMargins(5,0,0,5);

        userSignatureLabel->setFixedHeight(height-10);
        userSignatureLabel->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
        userSignatureLabel->setContentsMargins(0,0,0,5);
        calculateSignatureLengh(this->layout_mode);

        mainLayout_mode_1 = new QHBoxLayout;
        mainLayout_mode_1->addWidget(userIcon,1, Qt::AlignLeading);
        mainLayout_mode_1->addWidget(userNameLabel,1,  Qt::AlignLeft|Qt::AlignBottom);
        mainLayout_mode_1->addWidget(userSignatureLabel,6, Qt::AlignLeft|Qt::AlignBottom);
        mainLayout_mode_1->setSpacing(5);
        mainLayout_mode_1->setContentsMargins(10,5,5,5);

        this->setLayout(mainLayout_mode_1);
        miniStatusIcon->setFixedSize(userIcon->width()/3,userIcon->width()/3);
        miniStatusIcon->move(userIcon->x()+ userIcon->width()*3/4,userIcon->height()*3/4);
        setMiniStatusIcon(userStatus);
        miniStatusIcon->raise();
    }
}

int ContactItem::getLayoutMode()
{
    if(this->layout_mode)
        return this->layout_mode;
    else
        return 1;
}

QString ContactItem::getUserAcctNo()
{
    return this->userAcctNo;
}

QString ContactItem::getUserName()
{
    return this->userName;
}

void ContactItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        createContactMenu();
    }
    return QWidget::mousePressEvent(event);
}

void ContactItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        createConversationWindow(userAcctNo);
    }
    return QWidget::mousePressEvent(event);
}

void ContactItem::createContactMenu()
{
    contactItemMenu = new QMenu(this);
//    contactItemMenu->addAction("发送即时消息", createConversationWindow(userAcctNo));
//    contactItemMenu->addAction("发送电子邮件", createEmailWindow(userAcctNo));
//    contactItemMenu->addSeparator();
//    contactItemMenu->addAction("查看资料", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("分享他的名片", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("消息记录", createEmailWindow(userAcctNo));
//    contactItemMenu->addSeparator();
//    contactItemMenu->addAction("设置权限", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("修改备注姓名", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("移动联系人至", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("删除好友", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("举报此用户", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("好友管理", createEmailWindow(userAcctNo));
//    contactItemMenu->addSeparator();
//    contactItemMenu->addAction("会员快捷功能", createEmailWindow(userAcctNo));
//    contactItemMenu->addAction("进入QQ空间", createEmailWindow(userAcctNo));

    contactItemMenu->addAction("发送即时消息");
    contactItemMenu->addAction("发送电子邮件");
    contactItemMenu->addSeparator();
    contactItemMenu->addAction("查看资料");
    contactItemMenu->addAction("分享他的名片");
    contactItemMenu->addAction("消息记录");
    contactItemMenu->addSeparator();
    contactItemMenu->addAction("设置权限");
    contactItemMenu->addAction("修改备注姓名");
    contactItemMenu->addAction("移动联系人至");
    contactItemMenu->addAction("删除好友");
    contactItemMenu->addAction("举报此用户");
    contactItemMenu->addAction("好友管理");
    contactItemMenu->addSeparator();
    contactItemMenu->addAction("会员快捷功能");
    contactItemMenu->addAction("进入QQ空间");

    contactItemMenu->move(QCursor::pos());

    contactItemMenu->setStyleSheet("QMenu {"
                                   "background-color: white;"
                                   "border-radius:3px;"
                                   "margin:0px;"
                                   "border:1px solid lightgray;}"
                               "QMenu::item { "
                                "padding-left:20px;"
                                "padding-right:20px;"
                                "padding-top:5px;"
                                "padding-bottom:5px;"
                                "border-style:none; /* reserve space for selection border */\
                               }\
                               QMenu::item:selected {\
                                   background: lightgray;\
                               }"
                               "QMenu::separator {\
                                   height: 1px;\
                                   background: lightgray;\
                                   margin-left: 0px;\
                                   margin-right: 0px;\
                                }");
    //contactItemMenu->setWindowFlags(contactItemMenu->windowFlags()|Qt::NoDropShadowWindowHint);
    contactItemMenu->show();
    isMenuCreated = true;
    connect(contactItemMenu, &QMenu::aboutToHide, this, &ContactItem::updateMenuStatus);
}

void ContactItem::updateMenuStatus()
{
    isMenuCreated = false;
}

void ContactItem::createConversationWindow(QString userAcctNo)
{

}






